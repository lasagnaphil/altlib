//
// Created by lasagnaphil on 8/10/18.
//

#ifndef ALTLIB_UTILS_H
#define ALTLIB_UTILS_H

#ifndef M_PI
#define M_PI 3.1415926535
#endif

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


}


#endif //VULKAN_TUTORIAL_UTILS_H
