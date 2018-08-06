//
// Created by lasagnaphil on 7/15/18.
//

#ifndef THESYSTEM_RECT_H
#define THESYSTEM_RECT_H

#include "Vector2.h"
#include "../json/Json.h"

template <typename T>
struct Rect {
    T x1, y1;
    T x2, y2;

    static Rect create(T x1, T y1, T x2, T y2) {
        return Rect {.x1 = x1, .y1 = y1, .x2 = x2, .y2 = y2};
    }
    static Rect fromSize(T x, T y, T w, T h) {
        return Rect {.x1 = x, .y1 = y, .x2 = x+w, .y2 = y+h};
    }

    T width() { return x2 - x1; }
    T height() { return y2 - y1; }
    Vector2<T> origin() { return Vector2<T> {(x1 + x2)/2, (y1 + y2)/2}; }

    void deserialize(json::Value json);
    json::Value serialize();
};

using Rect2f = Rect<float>;
using Rect2i = Rect<int>;


#endif //THESYSTEM_RECT_H
