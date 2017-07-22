#pragma once
#include "vector3.h"
#include "ray.h"
#include "geometry.h"

#include <vector>

class Plane : public Geometry
{
public:
	Plane(Vector3 n_, Vector3 p_, Vector3 _color, materialType _material) : normal(n_), Geometry(p_, _color, _material)
	{
	}

	Vector3 normal;

	float intersect(const Ray &r) const;

	bool fastintersect(const Ray &r) const;

	Vector3 GetNormalAtPoint(Vector3 point) const;
};
