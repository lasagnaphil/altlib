//
// Created by lasagnaphil on 7/10/18.
//

#ifndef ALTLIB_VEC_H
#define ALTLIB_VEC_H

#include <cassert>
#include <cstring>

#define FOREACH(__vec, __v, CODE) \
for (int __i = 0; __i < __vec.size; ++__i) { \
    auto& __v = __vec.data[__i]; \
    CODE \
}

#define FOREACHI(__vec, __v, __i, CODE) \
for (int __i = 0; __i < __vec.size; ++__i) { \
    auto& __v = __vec[__i]; \
    CODE \
} \

template <typename T>
struct Slice {
    T* data;
    size_t size;

    static Slice fromRaw(T* data, size_t size) {
        return Slice {
            .data = data,
            .size = size
        };
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

template <typename T>
struct Vec {
    T* data;
    size_t size;
    size_t capacity;

    static Vec create(size_t capacity = 0) {
        Vec vec;
        vec.data = new T[capacity];
        vec.size = 0;
        vec.capacity = capacity;
        return vec;
    }
    static Vec emptyWithSize(size_t size) {
        Vec vec;
        vec.data = new T[size];
        vec.size = size;
        vec.capacity = size;
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
        Vec vec = Vec::create(count);
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

    Slice<T> toSlice() {
        return Slice<T>::fromRaw(data, size);
    }

    void free() {
        delete[] data;
    }

    void push(const T& elem) {
        if (size >= capacity) {
            reserve(capacity > 0? 2 * capacity : 1);
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

    bool empty() const {
        return size == 0;
    }
};


#endif //ALTLIB_VEC_H
