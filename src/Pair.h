//
// Created by lasagnaphil on 7/14/18.
//

#ifndef THESYSTEM_TUPLE_H
#define THESYSTEM_TUPLE_H

template <typename T, typename U>
struct Pair {
    T left;
    U right;

    static Pair create(T left, U right) {
        return {.left = left, .right = right};
    }

    void unpack(T& l, U& r) {
        l = left;
        r = right;
    }
};
#endif //THESYSTEM_TUPLE_H
