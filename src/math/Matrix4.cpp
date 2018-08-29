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

template<>
Matrix4f Matrix4f::lookAt(const Vector3f &eye, const Vector3f &center, const Vector3f &up) {

    Vector3f f = (eye - center).normalize();
    Vector3f r = up.cross(f).normalize();
    Vector3f u = f.cross(r);

    return Matrix4::create(
        r.x, r.y, r.z, -r.dot(eye),
        u.x, u.y, u.z, -u.dot(eye),
        f.x, f.y, f.z, -f.dot(eye),
        0, 0, 0, 1.0f
    );
}

