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
#include <iostream>

#include "vector3.h"
#include "ray.h"
#include "sphere.h"
#include "pointlight.h"

std::vector<Geometry *> Scene;

void SetupScene()
{


    Scene.push_back(new Sphere(1e3f, Vector3(-1e3f + 1,40.8,81.6), Vector3(.75f, .25f, .25f), Geometry::DIFFUSE));//Rght

    Scene.push_back(new Sphere(1e3f, Vector3(1e3f + 120, 40.8f, 81.6f), Vector3(.25f, .75f, .25f), Geometry::DIFFUSE));//lft

     Scene.push_back(new Sphere(1e3f, Vector3(50.f, 40.8f, 1e3f), Vector3(.25f, .25f, .75f),Geometry::DIFFUSE));//Back
    Scene.push_back(new Sphere(1e3f, Vector3(50.f, 40.8f, -1e3f + 170.f), Vector3(.25f, .25f, .25f), Geometry::DIFFUSE));//Frnt
   
     Scene.push_back(new Sphere(1e3f, Vector3(50.f, -1e3f, 81.6f), Vector3(.25f, .25f, .25f), Geometry::DIFFUSE));//Botm
    Scene.push_back(new Sphere(1e3f, Vector3(50.f, 1e3f + 91.6f, 81.6f), Vector3(.75f, .75f, .25f), Geometry::DIFFUSE));//Top
    
    Scene.push_back(new Sphere(16.5f, Vector3(27.f, 16.5f, 47.f), Vector3(.25f, .75f, .75f), Geometry::SPECULAR));//Mirr
    Scene.push_back(new Sphere(16.5f, Vector3(73.f, 16.5f, 78.f), Vector3(.75f, .75f, .75f), Geometry::DIFFUSE));//Mirr
    
    Scene.push_back(new PointLight(Vector3(60.f, 185.0f, 86.6f), Vector3(1.0f, 1.0f, 1.0f), Geometry::SPECULAR));
    

}

//ray vs all objects
inline bool Intersect(const Ray &r, float &t, int &id)
{
    //find the closest intersection that is not self
    float d;
    float inf = t = 1e20f;
    
    int arraySize = Scene.size() - 1;
    for (int i = arraySize; i >= 0; i--)
    {
        d = Scene[i]->intersect(r);
        if (d>0 && d < t)
        {
            t = d;
            id = i;
        }
    }
    return t<inf;
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

Vector3 Radiance(const Ray &r, int depth)
{
    Vector3 out;
    
    float t;
    int id = 0;
    if (!Intersect(r, t, id))
        return Vector3(); // if miss, return black
    
    const Geometry *obj = Scene[id];
    
    //break recursion at max bounce
    ++depth;
    if (depth > 4)
        return obj->emmission;
    
    Vector3 color = obj->color;
    
    Vector3 x = r.origin + r.direction*t;
    
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
    
    Ray reflect = Ray(x, reflected);
    Vector3 nextBounce = Radiance(reflect, depth);
    out = obj->emmission + color.Multiply(nextBounce);
    
    
    return out;
}

int main(int argc, char *argv[])
{
    
    if (argc < 4)
    {
        fprintf(stdout,"usage: Tracer width height samples\n");
        return 1;
    }
    
    SetupScene();
    
    int w = atoi(argv[1]), h = atoi(argv[2]), samps = atoi(argv[3]);
    
    fprintf(stdout, "%i %i %i",w,h,samps);
    
    Vector3 *pixels = new Vector3[w*h];
    
    Vector3 color;
    
    Ray cam(Vector3(50, 52, 295.6f), Vector3(0, -0.042612f, -1).Normalise()); // cam pos, dir
    
    Vector3 cx = Vector3(w*.5135f / h);
    Vector3 cy = (cx.CrossProduct(cam.direction)).Normalise()*.5135f;
    
    for (int y = 0; y < h; y++) //rows
    {
        
        fprintf(stderr, "\r %d/%d",y,h);
        
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
                        
                        color = color + Radiance(Ray(cam.origin + d * 140.f, d.Normalise()), 0)*(1.f / samps);
                        
                    }
                    int index = (h - y - 1)*w + x;
                    pixels[index].x = color.x;
                    pixels[index].y = color.y;
                    pixels[index].z = color.z;
                }
                
                
            }
        }
    }
    
    //write out image.
    FILE *f;
    f = fopen( "image.ppm", "w");
     
    if (f == 0)
    {
        fprintf(stderr, "Cannot open file %s","image.ppm");
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
    
    return 0;
}
