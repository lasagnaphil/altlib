//
// Created by lasagnaphil on 6/22/18.
//

#ifndef THESYSTEM_STACKVEC_H
#define THESYSTEM_STACKVEC_H

#include <cstddef>
#include <cassert>
#include <cstring>
#include <utility>

template <typename T, unsigned int N>
struct StackVec {
    T _data[N];
    size_t _size;

    static StackVec create() {
        StackVec vec;
        vec._size = 0;
        return vec;
    }

    static StackVec from(const T& elem, size_t count) {
        assert(count <= N);
        StackVec vec;
        for (size_t i = 0; i < count; ++i) {
            vec._data[i] = elem;
        }
        vec._size = count;
        return vec;
    }

    StackVec clone() {
        StackVec vec;
        vec._size = _size;
        memcpy(vec._data, _data, sizeof(T) * _size);
        return vec;
    }

    void push(const T& elem) {
        assert(_size < N);
        _data[_size] = elem;
        _size++;
    }

    T pop() {
        assert(_size > 0);
        _size--;
        return _data[_size];
    }

    T* getData() {
        return _data;
    }

    std::size_t size() const {
        return _size;
    }

    T& operator[](size_t idx) {
        assert(idx >= 0 && idx < _size);
        T& item = _data[idx];
        return item;
    }

    const T& operator[](size_t idx) const {
        assert(idx >= 0 && idx < _size);
        const T& item = _data[idx];
        return item;
    }
};


#endif //THESYSTEM_STACKVEC_H
