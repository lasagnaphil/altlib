//
// Created by lasagnaphil on 8/10/18.
//

#ifndef ALTLIB_UTILS_H
#define ALTLIB_UTILS_H

namespace utils {

template <typename T>
T max(T a, T b) {
    return (a > b)? a : b;
}

template <typename T>
T min(T a, T b) {
    return (a < b)? a : b;
}

template <typename T>
T clamp(T value, T lower, T upper) {
    return max(lower, min(value, upper));
}

}


#endif //VULKAN_TUTORIAL_UTILS_H
