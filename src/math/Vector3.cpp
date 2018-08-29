//
// Created by lasagnaphil on 8/30/18.
//

#include "Vector3.h"
#include <math.h>

template <>
Vector3f Vector3f::normalize() const {
    return *this / sqrtf(x * x + y * y + z * z);
}

template <>
float Vector3f::mag() const {
    return sqrtf(x * x + y * y + z * z);
}
