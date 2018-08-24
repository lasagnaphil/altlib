//
// Created by lasagnaphil on 7/22/18.
//

#ifndef ALTLIB_STRINGPOOL_H
#define ALTLIB_STRINGPOOL_H

#include "Map.h"
#include "Str.h"

struct Symbol;
struct StrView;
struct String;

struct StringPool {

    static StringPool inst;

    Map<Symbol, StrView> stringDB;
    Vec<String> allocStrs;

    static StringPool create();

    void free();

    Symbol add(const char* str);
    Symbol add(StrView str);

    Symbol heapAdd(const char* str);
    Symbol heapAdd(String str);

    Symbol getSym(StrView str) const;

    Symbol getSym(const char* str) const;

    StrView getStr(Symbol &sym) const;

    bool remove(Symbol& symbol);

    // implements FNV-1a hash algorithm
    size_t _hash(StrView string) const;
    size_t _hash(String string) const;
};



#endif //ALTLIB_STRINGPOOL_H
