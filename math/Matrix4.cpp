//
// Created by lasagnaphil on 7/16/18.
//

#include "Matrix4.h"

template <>
Matrix4f Matrix4f::rotateZ(float radians) {
    return Matrix4f::create(
        cosf(radians), -sinf(radians), 0, 0,
        sinf(radians), cosf(radians), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
}
