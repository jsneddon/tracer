#pragma once
#include <vector>
#include "geometry.h"

class SceneLoader
{
public:
	SceneLoader();
	bool Load(	char * filename,
				std::vector<Geometry *> &Scene,
				std::vector<Geometry *> &Lights
	);
};
