#include "sphere.h"

float Sphere::intersect(const Ray &r) const
{ // returns distance, 0 if nohit
    Vector3 op = position - r.origin; // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
    float t, eps = 1e-4f, b = op.DotProduct(r.direction), det = b*b - op.DotProduct(op) + radius*radius;
    if (det<0) 
		return 0; 
	else 
		det = sqrt(det);

    return (t = b - det)>eps ? t : ((t = b + det)>eps ? t : 0);
}

bool Sphere::fastintersect(const Ray &r) const
{
	Vector3 op = position - r.origin;

	float t;
	float eps = 1e-4f;
	float b = op.DotProduct(r.direction);
	float det = b*b - op.DotProduct(op) + radiusSq;

	if (det < 0)
		return false;

	return true;
}


Vector3 Sphere::GetNormalAtPoint(Vector3 point) const
{
    return(point - position).Normalise();
}

template <typename Type>
inline Type maMin(Type x, Type y)
{
	return (x) < (y) ? (x) : (y);
}

template <typename Type>
inline Type maMax(Type x, Type y)
{
	return (x) > (y) ? (x) : (y);
}

template <typename Type>
inline Type maClamp(Type a, Type min, Type max)
{
	return (a) < (min) ? (min) : (((a) > (max) ? (max) : (a)));
}

#if 0
bool LineSegmentVsSphereIntersection(Vector3 &intersectionPoint,
	const Vector3	  &lineStart,
	const Vector3	  &lineDirection,
	float				lineLength,
	const Vector3	  &sphereCentre,
	float				sphereRadiusSquared)
{
	Vector3 T;
	Vector3 closestPointOnLineSegment;
	float originalDot, dot;
	float distanceSquaredToClosestPoint;

	T = sphereCentre - lineStart;
	originalDot = lineDirection.DotProduct( T );
	
	dot = maMax(0.0f, originalDot);
	dot = maMin(lineLength, dot);

	closestPointOnLineSegment.x = lineStart.x + dot * lineDirection.x;
	closestPointOnLineSegment.y = lineStart.y + dot * lineDirection.y;
	closestPointOnLineSegment.z = lineStart.z + dot * lineDirection.z;

	distanceSquaredToClosestPoint =
		(closestPointOnLineSegment.x - sphereCentre.x) * (closestPointOnLineSegment.x - sphereCentre.x) +
		(closestPointOnLineSegment.y - sphereCentre.y) * (closestPointOnLineSegment.y - sphereCentre.y) +
		(closestPointOnLineSegment.z - sphereCentre.z) * (closestPointOnLineSegment.z - sphereCentre.z);
	// maVector3D_LengthSquared( &closestPointOnLineSegment );
	if (distanceSquaredToClosestPoint > sphereRadiusSquared)
		return false;

	
	{
		/* Find the closest intersection point (sorry for the lack of working)

		if MagnitudeSquared(v3LineStart + t * v3LineDirection - v3SphereCentre) = fSphereRadiusSquared,
		then what's t? Use the quadratic formula to find the intersection closest
		to v3LineStart
		*/

		/* const float A = 1.0f; */
		//			const float B = -2.0f * originalDot; // ( v3LineDirection->x * T.x + v3LineDirection->y * T.y + v3LineDirection->z * T.z );
		//			const float C = T.x * T.x + T.y * T.y + T.z * T.z - fSphereRadiusSquared;

		const float B = 2.0f * lineStart.x * lineDirection.x
			+ 2.0f * lineStart.y * lineDirection.y
			+ 2.0f * lineStart.z * lineDirection.z
			- 2.0f * lineDirection.x * sphereCentre.x
			- 2.0f * lineDirection.y * sphereCentre.y
			- 2.0f * lineDirection.z * sphereCentre.z;
		const float C = lineStart.x * lineStart.x
			+ lineStart.y * lineStart.y
			+ lineStart.z * lineStart.z
			+ sphereCentre.x * sphereCentre.x
			+ sphereCentre.y * sphereCentre.y
			+ sphereCentre.z * sphereCentre.z
			- 2.0f * lineStart.x * sphereCentre.x
			- 2.0f * lineStart.y * sphereCentre.y
			- 2.0f * lineStart.z * sphereCentre.z
			- sphereRadiusSquared;

		/* quadratic solution formula: (-B + sqrt(B*B-4.0f*A*C)) / (2.0f * A) or (-B - sqrt(B*B-4.0f*A*C)) / (2.0f * A) */
		float det = B * B - 4.0f * C;
		float sqrtDet, t0;

		if (det < 0.0f)
		{
			/* sometimes the math is a little inaccurate, and grazing lines can cause
			negative determinates. In this case, pretend there is no collision. */
			return false;
		}

		sqrtDet = sqrtf(det);
		t0 = (-B - sqrtDet) * 0.5f;			/* only interested in the smallest value at the moment */

		if (t0 < 0.0f)
		{
			/* the start point is inside the sphere... see if there's another intersection */
			float t1 = (-B + sqrtDet) * 0.5f;
			if (t1 < lineLength)
			{
				intersectionPoint= lineStart;

				maVector3D_IncrementScaled(intersectionPoint, lineDirection, t1);
				return true;
			}
			else
			{
				/* line is entirely within the sphere: it's still considered to be an intersection,
				but there is no intersection point... */
				intersectionPoint.x = 0.0f;
				intersectionPoint.y = 0.0f;
				intersectionPoint.z = 0.0f;

				return true;
			}
		}

		intersectionPoint= lineStart;
		maVector3D_IncrementScaled(intersectionPoint, lineDirection, t0);
		return true;
	}
}
#endif
