#pragma once

#include "vector3.h"

class Ray
{
public:
    
    Vector3 origin, direction;
    Ray(Vector3 oo, Vector3 dd) : origin(oo), direction(dd) {};
};
