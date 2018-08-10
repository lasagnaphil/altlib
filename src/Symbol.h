//
// Created by lasagnaphil on 7/22/18.
//

#ifndef THESYSTEM_SYMBOL_H
#define THESYSTEM_SYMBOL_H

#include <cstddef>
#include "String.h"
#include "StringPool.h"

struct Symbol {
    size_t index;

    inline static Symbol empty() {
        return Symbol {.index = 0};
    }

    inline bool operator==(Symbol other) const {
        return other.index == index;
    }
};

#endif //THESYSTEM_SYMBOL_H
