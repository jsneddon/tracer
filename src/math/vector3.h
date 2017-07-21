#pragma once
#include <math.h>

class Vector3
{
public:
    //data
    float x=0, y=0, z=0;
    
    Vector3(float xx = 0, float yy = 0, float zz = 0) : x(xx), y(yy), z(zz) {};
    //fn's
    float Length() const;
    
    Vector3 Multiply(const Vector3 &b) const;
    
    Vector3& Normalise();
    
    float DotProduct(const Vector3 &b) const;
    
    Vector3 CrossProduct(Vector3&b);
    
    //operators
    Vector3 operator+(const Vector3 &b) const;
    Vector3 operator-(const Vector3 &b) const;
    Vector3 operator*(float b) const;
    Vector3 operator/(float b) const;
};
