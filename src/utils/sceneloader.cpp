#include "sceneloader.h"
#include <fstream>
#include <sstream>      // std::istringstream
#include <string>
#include "sphere.h"
#include "plane.h"
#include "pointlight.h"


SceneLoader::SceneLoader()
{

}

size_t trimwhitespace(char *out, size_t len, const char *str)
{
	if (len == 0)
		return 0;

	const char *end;
	size_t out_size;

	// Trim leading space
	while (isspace((unsigned char)*str)) str++;

	if (*str == 0)  // All spaces?
	{
		*out = 0;
		return 1;
	}

	// Trim trailing space
	end = str + strlen(str) - 1;
	while (end > str && isspace((unsigned char)*end)) end--;
	end++;

	// Set output size to minimum of trimmed string length and buffer size minus 1
	out_size = (end - str) < len - 1 ? (end - str) : len - 1;

	// Copy trimmed string and add null terminator
	memcpy(out, str, out_size);
	out[out_size] = 0;

	return out_size;
}

void getData(char* out, char*in)
{
	char* loc = strchr(in, ':');
	loc++;
	if (loc != nullptr)
	{
		int len = strlen(loc);
		trimwhitespace(out, len, loc);
	}
}

void getVector(Vector3 &out, std::istringstream &iss)
{
	char x[32];
	char y[32];
	char z[32];

	iss >> x >> y >> z;
	out.x = atof(x);
	out.y = atof(y);
	out.z = atof(z);


}

bool SceneLoader::Load(char * filename,
	std::vector<Geometry *> &Scene,
	std::vector<Geometry *> &Lights
)
{
	
	std::ifstream in(filename);

	if (!in) 
	{
		fprintf(stderr, "Cannot open file");
		return 1;
	}

	Geometry * current = 0;

	char type[32];
	Vector3 position;
	Vector3 color;
	Vector3 normal;
	float radius;
	Geometry::materialType material;
	std::string line;
	char str[255];

	while (in.getline(str,255))
	{
		line.assign(str);
		std::istringstream iss(line);

		char token[32];
		iss >> token;
		switch (token[0])
		{
		case '}':
			//we have a complete object so lets create it!!!
			switch (type[0])
			{
			case 'S':
				Scene.push_back(
					new Sphere(
						radius, 
						position, 
						color, 
						material)
					);

				break;
			case 'P':
				Scene.push_back(
					new Plane(
						normal,
						position,
						color,
						material)
				);
				break;
			case 'L':

				PointLight * p =
					new PointLight(
						position,
						color,
						material);

				Scene.push_back(p);
				Lights.push_back(p);


				break;

			}


			break;
		case 't':
			//we have an object 
			//this should be its name
			iss >> type;
			fprintf(stderr, type);
		break;

		case 'p':
			getVector(position, iss);
			fprintf(stderr, "pos: %f %f %f ", position.x, position.y, position.z);
		break;

		case 'n':
			getVector(normal, iss);
			fprintf(stderr, "norm: %f %f %f ", normal.x, normal.y, normal.z);
			break;

		case 'c':
			getVector(color, iss);
			fprintf(stderr, "col: %f %f %f ", color.x, color.y, color.z);
			break;

		case 'r':
			char r[32];
			iss >> r;
			radius = atof(r);
			fprintf(stderr, "radius: %f ", radius);
			break;

		case 'm':
			char mat[32];
			iss >> mat;

			if (mat[0] == 'S')
				material = Geometry::materialType::SPECULAR;
			else if (mat[0] == 'F')
				material = Geometry::materialType::FLAT;
			else if (mat[0] == 'G')
				material = Geometry::materialType::GLASS;
			else if (mat[0] == 'P')
				material = Geometry::materialType::PHONG;
			else
				material = Geometry::materialType::DIFFUSE;
			
			fprintf(stderr, "mat: %s ", mat);
			break;

		default:
			break;
		}
		
			

	}

	in.close();


	return false;
}