//
// Created by lasagnaphil on 7/15/18.
//

#ifndef ALTLIB_MATRIX4_H
#define ALTLIB_MATRIX4_H

#include <cmath>
#include <cstring>
#include "Vector4.h"
#include "Vector3.h"

template <typename T>
struct Matrix4 {
    Vector4<T> p[4];

    static Matrix4 create(T x1, T x2, T x3, T x4,
                          T x5, T x6, T x7, T x8,
                          T x9, T x10, T x11, T x12,
                          T x13, T x14, T x15, T x16) {
        Matrix4 mat;
        mat.p[0] = Vector4<T>::create(x1, x5, x9, x13);
        mat.p[1] = Vector4<T>::create(x2, x6, x10, x14);
        mat.p[2] = Vector4<T>::create(x3, x7, x11, x15);
        mat.p[3] = Vector4<T>::create(x4, x8, x12, x16);
        return mat;
    }

    static Matrix4 create(T* data) {
        Matrix4 mat;
        memcpy(&mat, data, 16 * sizeof(float));
        return mat;
    }

    static Matrix4 scale(const Vector3<T>& size) {
        return Matrix4::create(
            size.x, 0, 0, 0,
            0, size.y, 0, 0,
            0, 0, size.z, 0,
            0, 0, 0, 1
        );
    }

    static Matrix4 trans(const Vector3<T>& disp) {
        return Matrix4::create(
            1, 0, 0, disp.x,
            0, 1, 0, disp.y,
            0, 0, 1, disp.z,
            0, 0, 0, 1
        );
    }

    static Matrix4 rotateZ(float radians);

    static Matrix4 one() {
        return Matrix4::create(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    }

    Vector4<T>& operator[](int i) { return p[i]; }
    const Vector4<T>& operator[](int i) const { return p[i]; }

};

template <typename T>
Matrix4<T> operator+(Matrix4<T> lhs, const Matrix4<T>& rhs) {
    Matrix4<T> m;
    m.p[0] = lhs.p[0] + rhs.p[0];
    m.p[1] = lhs.p[1] + rhs.p[1];
    m.p[2] = lhs.p[2] + rhs.p[2];
    m.p[3] = lhs.p[3] + rhs.p[3];
    return m;
}

template <typename T>
Matrix4<T> operator-(Matrix4<T> lhs, const Matrix4<T>& rhs) {
    Matrix4<T> m;
    m.p[0] = lhs.p[0] + rhs.p[0];
    m.p[1] = lhs.p[1] + rhs.p[1];
    m.p[2] = lhs.p[2] + rhs.p[2];
    m.p[3] = lhs.p[3] + rhs.p[3];
    return m;
}

template <typename T>
Matrix4<T> operator*(T k, const Matrix4<T>& rhs) {
    Matrix4<T> m;
    m.p[0] = k * rhs.p[0];
    m.p[1] = k * rhs.p[1];
    m.p[2] = k * rhs.p[2];
    m.p[3] = k * rhs.p[3];
    return m;
}

// TODO: perform loop unrolling on this?
template <typename T>
Matrix4<T> operator*(Matrix4<T> lhs, const Matrix4<T>& rhs) {
    Matrix4<T> m;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            m.p[i][j] = 0;
            for (int k = 0; k < 4; ++k) {
                m.p[i][j] += lhs.p[i][k] * rhs.p[k][j];
            }
        }
    }
    return m;
}

// TODO: perform loop unrolling on this?
template <typename T>
Matrix4<T> operator*(Matrix4<T> lhs, const Vector4<T>& rhs) {
    Vector4<T> v;
    for (int i = 0; i < 4; ++i) {
        v.p[i] = 0;
        for (int k = 0; k < 4; ++k) {
            v.p[i] += lhs.p[i][k] * v.p[k];
        }
    }
}

using Matrix4f = Matrix4<float>;

#endif //ALTLIB_MATRIX4_H
