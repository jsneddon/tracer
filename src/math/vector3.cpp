#include "vector3.h"
//fn's
float Vector3::Length() const
{
    return sqrtf(x*x + y*y + z*z);
}
Vector3 Vector3::Multiply(const Vector3 &b) const
{
    return Vector3(x*b.x, y*b.y, z*b.z);
}
Vector3& Vector3::Normalise()
{
    return *this = *this * (1 / Length());
}
float Vector3::DotProduct(const Vector3 &b) const
{
    return x*b.x + y*b.y + z*b.z;
}
Vector3 Vector3::CrossProduct(Vector3&b)
{
    return Vector3(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
}
//operators
Vector3 Vector3::operator+(const Vector3 &b) const
{
    return Vector3(x + b.x, y + b.y, z + b.z);
}
Vector3 Vector3::operator-(const Vector3 &b) const
{
    return Vector3(x - b.x, y - b.y, z - b.z);
}
Vector3 Vector3::operator*(float b) const
{
    return Vector3(x*b, y*b, z*b);
}
Vector3 Vector3::operator/(float b) const
{
    return Vector3(x / b, y / b, z / b);
}
