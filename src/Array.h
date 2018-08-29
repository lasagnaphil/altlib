//
// Created by lasagnaphil on 8/29/18.
//

#ifndef VULKAN_TUTORIAL_ARRAY_H
#define VULKAN_TUTORIAL_ARRAY_H

#include "stdlib.h"

template <typename T, size_t N>
struct Array {
    T data[N];

    inline size_t size() { return N; }
    inline T& operator[](size_t i) { return data[i]; }
    inline const T& operator[](size_t i) const { return data[i]; }
};

#endif //VULKAN_TUTORIAL_ARRAY_H
