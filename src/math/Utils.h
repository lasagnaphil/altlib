//
// Created by lasagnaphil on 8/10/18.
//

#ifndef ALTLIB_UTILS_H
#define ALTLIB_UTILS_H

#include <math.h>
#include "Vector3.h"
#include "Vector4.h"

namespace utils {

template <typename T>
inline T max(T a, T b) {
    return (a > b)? a : b;
}

template <typename T>
inline T min(T a, T b) {
    return (a < b)? a : b;
}

template <typename T>
inline T clamp(T value, T lower, T upper) {
    return max(lower, min(value, upper));
}

inline float radians(float deg) {
    return deg * M_PI / 180;
}

inline float degrees(float rad) {
    return rad * 180 / M_PI;
}

template <typename T>
T powi(T d, unsigned int i) {
    double res = 1.0;
    while (i > 0) {
        res *= d;
        i--;
    }
    return res;
}

template <typename T>
Vector4<T> catmullRom(double a) {
    Vector4<T> coeffs;
    coeffs.x = -0.5*a + a*a - 0.5*a*a*a;
    coeffs.y = 1 - 2.5*a*a + 1.5*a*a*a;
    coeffs.z = 0.5*a + 2*a*a - 1.5*a*a*a;
    coeffs.w = -0.5*a*a + 0.5*a*a*a;
    return coeffs;
}

// from http://www.realtimerendering.com/resources/GraphicsGems/gemsv/ch3-3/tricubic.c
template <typename T>
T triCubic(Vector3<T> p, T *volume, int xDim, int yDim, int zDim)
{
    int xyDim = xDim * yDim;

    int x = (int) p.x, y = (int) p.y, z = (int) p.z;
    if (x < 0 || x >= xDim || y < 0 || y >= yDim || z < 0 || z >= zDim)
        return (0);

    T dx = p.x - (T) x, dy = p.y - (T) y, dz = p.z - (T) z;
    T* pv = volume + (x - 1) + (y - 1) * xDim + (z - 1) * xyDim;

#define CUBE(x)   ((x) * (x) * (x))
#define SQR(x)    ((x) * (x))

    /* factors for Catmull-Rom interpolation */
    T u[4], v[4], w[4];
    T r[4], q[4];
    T vox = 0;

    u[0] = -0.5f * CUBE (dx) + SQR (dx) - 0.5f * dx;
    u[1] =  1.5f * CUBE (dx) - 2.5f * SQR (dx) + 1;
    u[2] = -1.5f * CUBE (dx) + 2 * SQR (dx) + 0.5f * dx;
    u[3] =  0.5f * CUBE (dx) - 0.5f * SQR (dx);

    v[0] = -0.5f * CUBE (dy) + SQR (dy) - 0.5f * dy;
    v[1] =  1.5f * CUBE (dy) - 2.5f * SQR (dy) + 1;
    v[2] = -1.5f * CUBE (dy) + 2 * SQR (dy) + 0.5f * dy;
    v[3] =  0.5f * CUBE (dy) - 0.5f * SQR (dy);

    w[0] = -0.5f * CUBE (dz) + SQR (dz) - 0.5f * dz;
    w[1] =  1.5f * CUBE (dz) - 2.5f * SQR (dz) + 1;
    w[2] = -1.5f * CUBE (dz) + 2 * SQR (dz) + 0.5f * dz;
    w[3] =  0.5f * CUBE (dz) - 0.5f * SQR (dz);

    for (int k = 0; k < 4; k++)
    {
        q[k] = 0;
        for (int j = 0; j < 4; j++)
        {
            r[j] = 0;
            for (int i = 0; i < 4; i++)
            {
                r[j] += u[i] * *pv;
                pv++;
            }
            q[k] += v[j] * r[j];
            pv += xDim - 4;
        }
        vox += w[k] * q[k];
        pv += xyDim - 4 * xDim;
    }
    return (T)(vox < 0 ? 0.0 : vox);

#undef CUBE
#undef SQR
}

}


#endif //VULKAN_TUTORIAL_UTILS_H
