//
// Created by lasagnaphil on 8/10/18.
//

#ifndef ALTLIB_UTILS_H
#define ALTLIB_UTILS_H

#ifndef M_PI
#define M_PI 3.1415926535
#endif

#include <math.h>

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

}


#endif //VULKAN_TUTORIAL_UTILS_H
