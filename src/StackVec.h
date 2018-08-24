//
// Created by lasagnaphil on 6/22/18.
//

#ifndef ALTLIB_STACKVEC_H
#define ALTLIB_STACKVEC_H

#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <utility>

template <typename T, unsigned int N>
struct StackVec {
    T data[N];
    size_t size;

    static StackVec create() {
        StackVec vec;
        vec.size = 0;
        return vec;
    }

    static StackVec from(const T& elem, size_t count) {
        assert(count <= N);
        StackVec vec;
        for (size_t i = 0; i < count; ++i) {
            vec.data[i] = elem;
        }
        vec.size = count;
        return vec;
    }

    StackVec clone() {
        StackVec vec;
        vec.size = size;
        memcpy(vec.data, data, sizeof(T) * size);
        return vec;
    }

    void push(const T& elem) {
        assert(size < N);
        data[size] = elem;
        size++;
    }

    T pop() {
        assert(size > 0);
        size--;
        return data[size];
    }

    T* getData() {
        return data;
    }

    T& operator[](size_t idx) {
        assert(idx >= 0 && idx < size);
        T& item = data[idx];
        return item;
    }

    const T& operator[](size_t idx) const {
        assert(idx >= 0 && idx < size);
        const T& item = data[idx];
        return item;
    }
};

#endif //ALTLIB_STACKVEC_H
