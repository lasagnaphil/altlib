//
// Created by lasagnaphil on 8/10/18.
//

#ifndef ALTLIB_SET_H
#define ALTLIB_SET_H

#include "Map.h"

#define SET_FOREACH(__set, __elem, __CODE) MAP_FOREACH(__set.items, __elem, __empty, __CODE)

template<typename T>
struct Set {
    struct Empty {};
    Map<T, Empty> items;

    static Set<T> create() {
        Set set;
        set.items = Map<T, Empty>::create();
        return set;
    }

    void free() { items.free(); }

    void reserve(size_t numElems) { items.reserve(numElems); }
    void clear() { items.clear(); }
    bool empty() const { return items.empty(); }
    size_t size() const { return items.size(); }

    void insert(const T& item) {
        items[item] = Empty {};
    }

    bool has(const T& item) const {
        return items.contains(item);
    }
};
#endif //ALTLIB_SET_H
