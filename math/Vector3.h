//
// Created by lasagnaphil on 7/15/18.
//

#ifndef THESYSTEM_VECTOR3_H
#define THESYSTEM_VECTOR3_H

template <typename T>
struct Vector3 {
    union {
        struct {
            T x, y, z;
        };
        T p[3];
    };

    static inline Vector3 create(T x, T y, T z) {
        Vector3 vec;
        vec.x = x; vec.y = y; vec.z = z;
        return vec;
    }
    static inline Vector3 zero() {
        Vector3 vec;
        vec.x = 0; vec.y = 0; vec.z = 0;
        return vec;
    }
    static inline Vector3 one() {
        Vector3 vec;
        vec.x = 1; vec.y = 1; vec.z = 1;
        return vec;
    }

    bool operator==(Vector3 other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    bool operator!=(Vector3 other) const {
        return !(x == other.x && y == other.y && z == other.z);
    }

    T& operator[](int i) { return p[i]; }
    const T& operator[](int i) const { return p[i]; }
};


template <typename T>
Vector3<T> operator+(Vector3<T> lhs, const Vector3<T>& rhs) {
    return Vector3<T> {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

template <typename T>
Vector3<T> operator-(Vector3<T> lhs, const Vector3<T>& rhs) {
    return Vector3<T> {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

template <typename T>
Vector3<T> operator*(Vector3<T> lhs, const Vector3<T>& rhs) {
    return Vector3<T> {lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z};
}

template <typename T>
Vector3<T> operator*(T k, const Vector3<T>& v) {
    return Vector3<T> {k * v.x, k * v.y, k * v.z};
}

using Vector3f = Vector3<float>;
using Vector3i = Vector3<int>;

#endif //THESYSTEM_VECTOR3_H
