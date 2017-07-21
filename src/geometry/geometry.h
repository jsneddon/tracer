#pragma once
#include "vector3.h"
#include "ray.h"
#include "geometry.h"
#include <vector>

class Geometry
{
public:
    Vector3 position;
    Vector3 emmission;
    
    //base color of the material
    Vector3 color = Vector3(.5, .5, .5);
    enum materialType { DIFFUSE, SPECULAR, GLASS, MAXTYPE };
    materialType material = DIFFUSE;
    
    Geometry(Vector3 _position,Vector3 _color, materialType _material ) : position(_position), color(_color), material(_material) {};
    virtual float intersect(const Ray &r) const = 0;
    virtual Vector3 GetNormalAtPoint(Vector3 point) const =0;
};
