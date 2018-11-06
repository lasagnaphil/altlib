//
// Created by lasagnaphil on 10/11/18.
//

#ifndef ALTLIB_QUEUE_H
#define ALTLIB_QUEUE_H

#include <cstddef>
#include <cstring>
#include <cassert>

template <typename T>
struct Queue {
    T* data;
    size_t capacity;
    size_t front;
    size_t size;

    static Queue create(size_t capacity = 0) {
        Queue queue;
        queue.data = new T[capacity];
        queue.capacity = capacity;
        queue.front = 0;
        queue.size = 0;
        return queue;
    }

    void free() {
        delete[] data;
    }

    void enq(T elem) {
        if (size == capacity) {
            reserve(capacity == 0? 1 : 2 * capacity);
        }
        size_t newIdx = (front + size) % capacity;
        data[newIdx] = elem;
        size++;
    }

    T deq() {
        assert(size > 0);
        T item = data[front];
        front = (front + 1) % capacity;
        size--;
        return item;
    }

    void reserve(size_t newCapacity) {
        if (newCapacity <= capacity) { return; }
        T* newData = new T[newCapacity];
        memcpy(newData, data, sizeof(T)*capacity);
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

    bool empty() const {
        return size == 0;
    }
};

#endif //ALTLIB_QUEUE_H
