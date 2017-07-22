#include "plane.h"

float Plane::intersect(const Ray &r) const
{

	float denom = normal.DotProduct(r.direction);
	if (abs(denom) > 0.0001f) // your favorite epsilon
	{
		float t = (position - r.origin).DotProduct(normal) / denom;
		if (t >= 0)
			return t; // you might want to allow an epsilon here too
	}

	return 0.0f;
}

bool Plane::fastintersect(const Ray &r) const
{
	return false;
}


Vector3 Plane::GetNormalAtPoint(Vector3 point) const
{
	return normal;
}

/*
typedef Vector3 maVector3D;
typedef Vector4 maVector4D;

float				maLinearInterpolateReal(float alpha, float zero, float one)
{
	return zero + alpha * (one - zero);
}
void		maVector3D_LinearInterpolate(maVector3D			  * pResult,
	float				fAlpha,
	const maVector3D		  * pZero,
	const maVector3D		  * pOne)
{
	pResult->x = maLinearInterpolateReal(fAlpha, pZero->x, pOne->x);
	pResult->y = maLinearInterpolateReal(fAlpha, pZero->y, pOne->y);
	pResult->z = maLinearInterpolateReal(fAlpha, pZero->z, pOne->z);
}

float		maGeometryMaths_SignedDistanceFromPlane(const maVector3D	  * point,
	const maVector4D	  * plane)
{
	return
		point->x * plane->x +
		point->y * plane->y +
		point->z * plane->z -
		plane->w;
}

bool		maGeometryMaths_CalculateIntersectionOfLineWithPlane(maVector3D			  * result,
	float				  * timeOfIntersection,
	const maVector3D	  * linePointA,
	const maVector3D	  * linePointB,
	const maVector4D	  * plane)
{
	float a = maGeometryMaths_SignedDistanceFromPlane(linePointA, plane);
	float b = maGeometryMaths_SignedDistanceFromPlane(linePointB, plane);
	if (a == b)
		return false;

	if (timeOfIntersection)
	{
		*timeOfIntersection = a / (a - b);
		maVector3D_LinearInterpolate(result, *timeOfIntersection, linePointA, linePointB);
	}
	else
		maVector3D_LinearInterpolate(result, a / (a - b), linePointA, linePointB);
	return true;
}
*/
