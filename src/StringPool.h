//
// Created by lasagnaphil on 7/22/18.
//

#ifndef THESYSTEM_STRINGPOOL_H
#define THESYSTEM_STRINGPOOL_H

#include "Map.h"
#include "String.h"

struct Symbol;
struct ImString;
struct OwnedString;

struct StringPool {

    static StringPool inst;

    Map<Symbol, ImString> stringDB;
    Vec<OwnedString> allocStrs;

    static StringPool create();

    void free();

    Symbol add(const char* str);
    Symbol add(ImString str);

    Symbol heapAdd(const char* str);
    Symbol heapAdd(OwnedString str);

    Symbol getSym(ImString str) const;

    Symbol getSym(const char* str) const;

    ImString getString(Symbol& sym) const;

    bool remove(Symbol& symbol);

    // implements FNV-1a hash algorithm
    size_t _hash(const ImString& string) const;
    size_t _hash(const OwnedString& string) const;
};



#endif //THESYSTEM_STRINGPOOL_H
