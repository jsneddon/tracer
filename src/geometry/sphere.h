#pragma once
#include "vector3.h"
#include "ray.h"
#include "geometry.h"

#include <vector>

class Sphere : public Geometry
{
public:
    float radius;
    
    Sphere(float rad_, Vector3 p_, Vector3 color, materialType _material) :
    Geometry(p_, color, _material), radius(rad_)
    {
    }
    
    float intersect(const Ray &r) const;
    Vector3 GetNormalAtPoint(Vector3 point) const;
};
