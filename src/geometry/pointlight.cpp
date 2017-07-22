#include "pointlight.h"

Vector3 PointLight::GetDirection(Vector3 &HitPoint)
{
    return Vector3(position.x - HitPoint.x, position.y - HitPoint.y, position.z - HitPoint.z).Normalise();
};

float PointLight::intersect(const Ray &r) const { // returns distance, 0 if nohit
    float radius = 5.0f;
    Vector3 op = position - r.origin; // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
    float t, eps = 1e-4f, b = op.DotProduct(r.direction), det = b*b - op.DotProduct(op) + radius*radius;
    if (det<0) return 0; else det = sqrt(det);
    return (t = b - det)>eps ? t : ((t = b + det)>eps ? t : 0);
}

Vector3 PointLight::GetNormalAtPoint(Vector3 point) const
{
    return(point - position).Normalise();
}
