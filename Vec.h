//
// Created by lasagnaphil on 7/10/18.
//

#ifndef THESYSTEM_VEC_H
#define THESYSTEM_VEC_H

#include <cassert>
#include <cstring>

#define VEC_FOREACH(__vec, __v, CODE) { \
    for (int __i = 0; __i < __vec.size; ++__i) { \
        auto& __v = __vec[__i]; \
        CODE \
    } \
}

#define VEC_FOREACHI(__vec, __v, __i, CODE) { \
    for (int __i = 0; __i < __vec.size; ++__i) { \
        auto& __v = __vec[__i]; \
        CODE \
    } \
}

template <typename T>
struct Vec {

    T* data;
    size_t size;
    size_t capacity;

    static Vec create(size_t capacity = 1) {
        Vec vec;
        vec.data = new T[capacity];
        vec.size = 0;
        vec.capacity = capacity;
        return vec;
    }
    static Vec fromElem(const T& elem, size_t count) {
        Vec vec;
        vec.data = new T[count];
        for (size_t i = 0; i < count; ++i) {
            vec.data[i] = elem;
        }
        vec.size = count;
        vec.capacity = count;
        return vec;
    }

    static Vec fromRaw(T* data, size_t count) {
        Vec vec;
        memcpy(vec.data, data, sizeof(T) * count);
        vec.size = count;
        return vec;
    }

    Vec<T> clone() {
        Vec vec = Vec::create(capacity);
        std::memcpy(vec.data, data, sizeof(T) * capacity);
        vec.size = size;
        return vec;
    }

    void free() {
        delete[] data;
    }

    void push(const T& elem) {
        if (size >= capacity) {
            reserve(2 * capacity);
        }
        data[size] = elem;
        size++;
    }

    T pop() {
        assert(size > 0);
        size--;
        return data[size];
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

    void reserve(size_t newCapacity) {
        if (newCapacity <= capacity) { return; }
        T* newData = new T[newCapacity];
        memcpy(newData, data, sizeof(T) * capacity);
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

    void shrink() {
        shrink(size);
    }

    void shrink(size_t newCapacity) {
        if (newCapacity >= capacity) { return; }
        T* newData = new T[newCapacity];
        memcpy(newData, data, sizeof(T) * newCapacity);
        delete[] data;
        data = newData;
        capacity = newCapacity;
        if (size > capacity) {
            size = capacity;
        }
    }
};


#endif //THESYSTEM_VEC_H
