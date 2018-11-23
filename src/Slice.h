//
// Created by lasagnaphil on 2018-11-13.
//

#ifndef THESYSTEM_SLICE_H
#define THESYSTEM_SLICE_H

#include <stddef.h>

template <typename T>
struct Slice {
    T* data;
    size_t size;

    static Slice fromRaw(T* data, size_t size) {
        return Slice {
            data,
            size
        };
    }

    static Slice fromRaw(const T* data, size_t size) {
        return Slice {
            data,
            size
        };
    }

    T& operator[](size_t idx) {
        assert(idx >= 0 && idx < size);
        return data[idx];
    }

    const T& operator[](size_t idx) const {
        assert(idx >= 0 && idx < size);
        return data[idx];
    }
};

#endif //THESYSTEM_SLICE_H
