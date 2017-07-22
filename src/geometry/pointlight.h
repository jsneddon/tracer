#pragma once
#include "vector3.h"
#include "ray.h"
#include "geometry.h"

#include <vector>

class Light : public Geometry
{
public:
    Light(Vector3 pos, Vector3 _color, materialType _material) : Geometry(pos, _color,_material) {
        emmission.x = 12.0;
        emmission.y = 12.0;
        emmission.z = 12.0;
    };
    virtual Vector3 GetDirection(Vector3 &HitPoint)=0;
    
};

class PointLight: public Light
{
public:
    PointLight(Vector3 pos, Vector3 _color, materialType _material) : Light(pos,_color,_material) {};
    
    Vector3 GetDirection(Vector3 &HitPoint) ;
    
    float intersect(const Ray &r) const;

	bool fastintersect(const Ray &r) const;

    Vector3 GetNormalAtPoint(Vector3 point) const;
};
