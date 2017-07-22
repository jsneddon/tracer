// tracer.cpp
// quick and dirty ray tracer example
// want to build something to test our amazon build tools
// and also compute tools.
// very much inspired by smallpt

#define _USE_MATH_DEFINES
#include <math.h>

#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <time.h>
#include <iostream>

#include "vector3.h"
#include "ray.h"
#include "sphere.h"
#include "pointlight.h"
#include "plane.h"
#include "sceneloader.h"



std::vector<Geometry *> Scene;
std::vector<Geometry *> Lights;

void SetupScene()
{
	SceneLoader l;
	l.Load("scene1.txt", Scene, Lights);
}



//ray vs all objects
inline bool Intersect(const Ray &r, float &t, int &id, int ignore=-1)
{
    float d;
    float inf = t = 1e20f;
    
    int arraySize = Scene.size() - 1;
    for (int i = arraySize; i >= 0; i--)
    {
		if (ignore == i)
			continue;

        d = Scene[i]->intersect(r);
        if (d>0 && d < t)
        {
            t = d;
            id = i;
        }
    }
    return t<inf;
}

inline bool IntersectQuick(const Ray &r, float &t, int &id, int ignore = -1, int ignore2 = -1)
{
	//find the closest intersection that is not self
	float d;
	float inf = t = 1e20f;

	int arraySize = Scene.size() - 1;
	for (int i = arraySize; i >= 0; i--)
	{
		if (ignore == i)
			continue;
		if (ignore2 == i)
			continue;

		if (Scene[i]->fastintersect(r) == true)
		{
			return true;
		}
	}
	return false;
}

inline bool PointIsInDirectLight(const Vector3 &p,int _id)
{
	int arraySize = Lights.size();
	for (int i = 0; i < arraySize; ++i)
	{
		Vector3 dir = Lights[i]->position-p;
		dir.Normalise();
		Ray r(p, dir);
		float t=0.0f;
		int id=0;
		if (IntersectQuick(r, t, id,_id,8))
		{
			return false;
		}

	}
	return true;
}

inline float randFloat()
{
    return (rand() % 1001 / 1000.0f);
}

inline float clamp(float x) { return x<0.f ? 0.f : x>1.f ? 1.f : x; }

//convert float gamma 2.2 to rgb 255
inline int toInt(float x) { return int(pow(clamp(x), 1 / 2.2f) * 255 + .5); }

void Reflect(Vector3 &out, const Vector3 &incidentVec, const Vector3 &normal)
{
    out = incidentVec - normal * normal.DotProduct(incidentVec) * 2.0f;
}

inline void Refract(
                    Vector3 &out, const Vector3 &incidentVec, const Vector3 &normal, float eta)
{
    float N_dot_I = normal.DotProduct(incidentVec);
    float k = 1.f - eta * eta * (1.f - N_dot_I * N_dot_I);
    if (k < 0.f)
        out = Vector3(0.f, 0.f, 0.f);
    else
        out = incidentVec * eta - normal * (eta * N_dot_I + sqrtf(k));
}

inline void Diffuse(
                    Vector3 &out, const Vector3 &incidentVec, const Vector3 &normal)
{
    float r1 = 2.f * 3.14159265f*randFloat();
    
    float r2 = randFloat();
    float r2s = sqrtf(r2);
    
    Vector3 nl = normal.DotProduct(incidentVec) < 0 ? normal : normal*-1;
    
    Vector3 w = nl;
    Vector3 u = ((fabs(w.x) > .1 ? Vector3(0, 1, 0) : Vector3(1, 0, 0)).CrossProduct(w)).Normalise();
    Vector3 v = w.CrossProduct(u);
    out = (u*cos(r1)*r2s + v*sin(r1)*r2s + w*sqrtf(1 - r2)).Normalise();
}

float clamp(float in, float min, float max)
{
	if (in > max)
		in = max;
	if (in , min)
		in = min;

	return in;
}
// returns intensity of diffuse reflection
float diffuseLighting(Vector3 N, Vector3 L)
{
	// calculation as for Lambertian reflection
	float diffuseTerm = clamp(N.DotProduct( L) , 0, 1);
	return diffuseTerm;
}

// returns intensity of specular reflection
float specularLighting(Vector3 N, Vector3 L, Vector3 V)
{
	float specularTerm = 0;

	// calculate specular reflection only if
	// the surface is oriented to the light source
	if (N.DotProduct( L) > 0)
	{
		// half vector
		Vector3 H = (L + V).Normalise();
		specularTerm = pow(N.DotProduct( H), 20.0f);
	}
	return specularTerm;
}

Vector3 Phong(const Geometry *obj,Vector3 o_toLight, Vector3 o_toCamera, Vector3 o_normal)
{
	// normalize vectors after interpolation
	Vector3 L = o_toLight.Normalise();
	Vector3 V = o_toCamera.Normalise();
	Vector3 N = o_normal;

	// get Blinn-Phong reflectance components
	float Iamb = 0.0f;
	float Idif = diffuseLighting(N, L);
	float Ispe = specularLighting(N, L, V);

	// diffuse color of the object from texture
	//Vector3 diffuseColor = texture(u_diffuseTexture, o_texcoords).rgb;

	// combination of all components and diffuse color of the object
	Vector3 result =  obj->color * Idif + obj->color*Ispe;

	return result;
}


inline Vector3 Phong(const Geometry *obj, const Ray &ray,Vector3 hit, Vector3 &normal)
{
	Vector3 color;
	int arraySize = Lights.size();
	for (int i = 0; i < arraySize; ++i)
	{
		
		Vector3 toLight = Lights[i]->position- hit;
		Vector3 toCam = ray.origin - hit;

		//diffuse
		float distToLight = toLight.Length();
		//toLight.Normalise();
		if (distToLight > 0.0f)
			toLight = toLight / distToLight;

		float NDotL = toLight.DotProduct(normal);
		Vector3 diff = obj->color * NDotL;

		//spec
		toCam.Normalise();
		Vector3 Halfway = (toLight + toCam).Normalise();

		float NDotH = Halfway.DotProduct(normal);
		Vector3 spec = obj->color * powf(NDotH,20.0f);

		clamp(distToLight, 0, 200.0f);
		float delta = 200 - distToLight;
		delta /= 200.0f;

		delta = delta*delta*delta;
		float attenuation = delta;

		color = color + ((diff + spec)*attenuation);
		//) * attenuation);
		//color = color + Phong(obj, toLight, toCam, normal);
		
	}
	return color*1/ arraySize;
}

Vector3 Radiance(const Ray &r, int depth,int bounces=3)
{
    Vector3 out;
    
    float t;
    int id = 0;
    if (!Intersect(r, t, id))
        return Vector3(); // if miss, return black
    
    const Geometry *obj = Scene[id];
    
    //break recursion at max bounce or hit light
    ++depth;
    if (depth > bounces || obj->emmission.x > 1.0f )
        return obj->emmission;
    
    Vector3 color = obj->color;
    
    Vector3 x = r.origin + r.direction*t;
    
	
	//too slow
	//direct line to light does exists
	// you are in some from of shadow
	/*
	if ( PointIsInDirectLight(x,id) == false )
	{
		//return Vector3();
	}
	else
	{
		bounces = 1;
	}
	*/
	

    //this assumes sphere
    Vector3	n = obj->GetNormalAtPoint(x);
    
    Vector3 normal = n;
    
    Vector3 reflected;
    
    if (obj->material == Geometry::DIFFUSE)
        Diffuse(reflected, r.direction, normal);//,1.3);
	if (obj->material == Geometry::SPECULAR)
		Reflect(reflected, r.direction, normal);//,1.3);
    if (obj->material == Geometry::GLASS)
        Refract(reflected, r.direction, normal, 0.0f);//,1.3);
	if (obj->material == Geometry::FLAT)
		return obj->color;
	if (obj->material == Geometry::PHONG)
		return Phong(obj, r, x, normal);

	//exit occasionally (80% of rays are a miss anyway)
	//if (rand() % 2 == 0)
		//return obj->emmission;

    Ray reflect = Ray(x, reflected);
    Vector3 nextBounce = Radiance(reflect, depth, bounces);
    out = obj->emmission + color.Multiply(nextBounce);
    
    
    return out;
}

int main(int argc, char *argv[])
{
    
    if (argc < 5)
    {
        fprintf(stdout,"usage: Tracer width height samples bounces\n");
        return 1;
    }

	//timing info
	time_t start_t, end_t;
	double diff_t;
	time(&start_t);
	
    
    SetupScene();
    
    int w = atoi(argv[1]), h = atoi(argv[2]), samps = atoi(argv[3]),bounces=atoi(argv[4]);
    
    fprintf(stdout, "%i %i %i",w,h,samps);
    
    Vector3 *pixels = new Vector3[w*h];
    
    Vector3 color;
    
    Ray cam(Vector3(50, 52, 295.6f), Vector3(0, -0.042612f, -1).Normalise()); // cam pos, dir
    
    Vector3 cx = Vector3(w*.5135f / h);
    Vector3 cy = (cx.CrossProduct(cam.direction)).Normalise()*.5135f;


	
	#pragma omp parallel for schedule(dynamic, 1) private(color)
	for (int y = 0; y < h; y++) //rows
	{
	//	int p = (hits * 100) / (hits+misses+1);
		/// (hits + misses + 1);
		if(y%10==0)
			fprintf(stderr, "\r %d/%d", y, h);

		for (int x = 0; x < w; x++)
		{
			for (int sy = 0; sy < 2; sy++)
			{
				for (int sx = 0; sx < 2; sx++)
				{
					color = Vector3();
					for (int s = 0; s < samps; s++)
					{
						float r1 = 2 * randFloat();
						float dx = r1 < 1 ? sqrtf(r1) - 1 : 1 - sqrtf(2 - r1);

						float r2 = 2 * randFloat();
						float dy = r2 < 1 ? sqrtf(r2) - 1 : 1 - sqrtf(2 - r2);


						Vector3 d = cx*(((sx + .5f + dx) / 2.f + x) / w - .5f) +
							cy*(((sy + .5f + dy) / 2 + y) / h - .5f) + cam.direction;
						Vector3 col = Radiance(Ray(cam.origin + d * 140.f, d.Normalise()), 0, bounces)*(1.f / samps);
				

						color = color + col;

					}
					int index = (h - y - 1)*w + x;
					pixels[index].x = color.x;
					pixels[index].y = color.y;
					pixels[index].z = color.z;
				}


			}
		}
	}


	time(&end_t);
	diff_t = difftime(end_t, start_t);

    //write out image.
	char file[32];
	sprintf(file, "image_w%i_h%i_s%i_b%i.ppm", w, h, samps,bounces);
    FILE *f;
    f = fopen( file, "w");
     
    if (f == 0)
    {
        fprintf(stderr, "Cannot open file %s",file);
    }
    else
    {
        fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
        for (int i = 0; i < w*h; i++)
        {
            fprintf(f, "%d %d %d ", toInt(pixels[i].x), toInt(pixels[i].y), toInt(pixels[i].z));
        }
    }
    
    fclose(f);

	f = fopen("stats.txt", "a");
	if (f == 0)
	{
		fprintf(stderr, "Cannot open file %s", "stats.txt");
	}
	else
	{
		fprintf(f, "\n%s w:%d h:%d s:%d b:%d t:%f", file, w, h, samps, bounces, diff_t);
	}
	fclose(f);

    return 0;
}
