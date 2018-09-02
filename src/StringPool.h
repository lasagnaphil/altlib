//
// Created by lasagnaphil on 7/22/18.
//

#ifndef ALTLIB_STRINGPOOL_H
#define ALTLIB_STRINGPOOL_H

#include "Map.h"
#include "Vec.h"
#include "strpool.h"

#define SYM(__str) StringPool::inst.inject(__str)
#define STR(__sym) StringPool::inst.getStr(__sym)

struct StrView;
struct String;

using Symbol = STRPOOL_U64;

struct StringPool {

    static StringPool inst;

    strpool_t pool;
    strpool_config_t config;

    static StringPool create();

    void free();

    Symbol inject(const char* str);
    Symbol inject(StrView str);

    StrView getStr(Symbol sym) const;

    void remove(Symbol symbol);
};



#endif //ALTLIB_STRINGPOOL_H
