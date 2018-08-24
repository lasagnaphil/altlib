//
// Created by lasagnaphil on 7/22/18.
//

#ifndef ALTLIB_SYMBOL_H
#define ALTLIB_SYMBOL_H

#include <stddef.h>
#include "Str.h"
#include "StringPool.h"

struct Symbol {
    size_t index;

    inline static Symbol empty() {
        return Symbol {0};
    }

    inline bool operator==(Symbol other) const {
        return other.index == index;
    }
};

#endif //ALTLIB_SYMBOL_H
