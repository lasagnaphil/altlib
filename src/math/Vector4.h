//
// Created by lasagnaphil on 7/15/18.
//

#ifndef ALTLIB_VECTOR4_H
#define ALTLIB_VECTOR4_H

template <typename T>
struct Vector4 {
    union {
        struct {
            T x, y, z, w;
        };
        T p[4];
    };
    static inline Vector4 create(T x, T y, T z, T w) {
        Vector4 vec;
        vec.x = x; vec.y = y; vec.z = z; vec.w = w;
        return vec;
    }
    static inline Vector4 fromRaw(T* arr) {
        Vector4 vec;
        vec.p[0] = arr[0]; vec.p[1] = arr[1]; vec.p[2] = arr[2]; vec.p[3] = arr[3];
        return vec;
    }
    static inline Vector4 zero() {
        Vector4 vec;
        vec.x = 0; vec.y = 0; vec.z = 0; vec.w = 0;
        return vec;
    }

    bool operator==(const Vector4& rhs) const {
        return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
    }
    bool operator!=(const Vector4& rhs) const {
        return !(x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
    }
    Vector4& operator+=(const Vector4& rhs) const {
        x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w;
        return *this;
    }
    Vector4& operator-=(const Vector4& rhs) const {
        x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w;
        return *this;
    }
    Vector4& operator*=(const Vector4& rhs) const {
        x *= rhs.x; y *= rhs.y; z *= rhs.z; w *= rhs.w;
        return *this;
    }
    Vector4& operator/=(const Vector4& rhs) const {
        x /= rhs.x; y /= rhs.y; z /= rhs.z; w /= rhs.w;
        return *this;
    }

    T& operator[](int i) { return p[i]; }
    const T& operator[](int i) const { return p[i]; }

    T innerProduct(const Vector4& rhs) {
        return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
    }
};

template <typename T>
Vector4<T> operator+(Vector4<T> lhs, const Vector4<T>& rhs) {
    return Vector4<T> {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w};
}

template <typename T>
Vector4<T> operator-(Vector4<T> lhs, const Vector4<T>& rhs) {
    return Vector4<T> {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w};
}

template <typename T>
Vector4<T> operator*(Vector4<T> lhs, const Vector4<T>& rhs) {
    return Vector4<T> {lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w};
}

template <typename T>
Vector4<T> operator*(T k, const Vector4<T>& v) {
    return Vector4<T> {k * v.x, k * v.y, k * v.z, k * v.w};
}

using Vector4f = Vector4<float>;
using Vector4i = Vector4<int>;
using Vector4d = Vector4<double>;

#endif //ALTLIB_VECTOR4_H
