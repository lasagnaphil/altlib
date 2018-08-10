//
// Created by lasagnaphil on 7/15/18.
//

#ifndef ALTLIB_VECTOR2_H
#define ALTLIB_VECTOR2_H

#include "../json/Json.h"

template <typename T>
struct Vector2 {
    union {
        struct {
            T x, y;
        };
        T p[2];
    };
    static inline Vector2 create(T x, T y) {
        Vector2 vec;
        vec.x = x; vec.y = y;
        return vec;
    }
    static inline Vector2 zero() {
        Vector2 vec;
        vec.x = 0; vec.y = 0;
        return vec;
    }
    static inline Vector2 one() {
        Vector2 vec;
        vec.x = 1; vec.y = 1;
        return vec;
    }

    bool operator==(Vector2 other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(Vector2 other) const {
        return !(x == other.x && y == other.y);
    }
    T& operator[](int i) { return p[i]; }
    const T& operator[](int i) const { return p[i]; }

    void deserialize(json::Value json);

    json::Value serialize();
};

template <typename T>
Vector2<T> operator+(Vector2<T> lhs, const Vector2<T>& rhs) {
    return Vector2<T> {lhs.x + rhs.x, lhs.y + rhs.y};
}

template <typename T>
Vector2<T> operator-(Vector2<T> lhs, const Vector2<T>& rhs) {
    return Vector2<T> {lhs.x - rhs.x, lhs.y - rhs.y};
}

template <typename T>
Vector2<T> operator*(Vector2<T> lhs, const Vector2<T>& rhs) {
    return Vector2<T> {lhs.x * rhs.x, lhs.y * rhs.y};
}

template <typename T>
Vector2<T> operator*(T k, const Vector2<T>& v) {
    return Vector2<T> {k * v.x, k * v.y};
}

using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;

#endif //ALTLIB_VECTOR2_H
