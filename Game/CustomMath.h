#ifndef CUSTOMMATH_H
#define CUSTOMMATH_H

#include <glm/glm.hpp>

static bool IsBigEndian()
{
    int a = 1;
    return !((char*)&a)[0];
}

static int ConvertToInt(char* buffer, int len)
{
    int a = 0;
    if (!IsBigEndian())
        for (int i = 0; i<len; i++)
            ((char*)&a)[i] = buffer[i];
    else
        for (int i = 0; i<len; i++)
            ((char*)&a)[3 - i] = buffer[i];
    return a;
}

class Quaternion
{
public:
    Quaternion(const glm::vec3 &axis, float angle);
    // Spherical Linear Interpolation
    static Quaternion Slerp(const Quaternion &a, const Quaternion &b, float f);
    // Concatenate (rotate by q followed by p, uses Grassmann product pq)
    static Quaternion Concatenate(const Quaternion& q, const Quaternion& p);
    // v = (0, 0, 0); s = 1
    static const Quaternion Identity;

private:

};

#endif