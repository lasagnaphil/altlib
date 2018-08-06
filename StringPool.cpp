//
// Created by lasagnaphil on 7/22/18.
//

#include "StringPool.h"
#include "Symbol.h"

StringPool StringPool::inst = StringPool::create();

StringPool StringPool::create() {
    StringPool pool;
    pool.stringDB = Map<Symbol, ImString>::create();
    pool.allocStrs = Vec<OwnedString>::create(16);
    return pool;
}

void StringPool::free() {
    stringDB.free();
    for (int i = 0; i < allocStrs.size; ++i) {
        allocStrs[i].free();
    }
    allocStrs.free();
}

Symbol StringPool::add(const char *str) {
    ImString imstr = ImString::create(str);
    return add(imstr);
}

Symbol StringPool::add(ImString str) {
    Symbol symbol {.index = _hash(str)};
    stringDB.insert(symbol, str);
    return symbol;
}

Symbol StringPool::heapAdd(const char *str) {
    auto ownedStr = OwnedString::create(str);
    allocStrs.push(ownedStr);
    Symbol symbol {.index = _hash(ownedStr)};
    stringDB.insert(symbol, ImString::create(ownedStr.data));
    return symbol;
}

Symbol StringPool::heapAdd(OwnedString str) {
    allocStrs.push(str);
    Symbol symbol {.index = _hash(str)};
    stringDB.insert(symbol, ImString::create(str.data));
    return symbol;
}

Symbol StringPool::getSym(ImString str) const {
    return Symbol {.index = _hash(str)};
}

Symbol StringPool::getSym(const char *str) const {
    ImString imstr = ImString::create(str);
    return Symbol {.index = _hash(imstr)};
}

ImString StringPool::getString(Symbol& sym) const {
    if (sym.index > 0) {
        return stringDB.get(sym);
    }
    else {
        return ImString::empty();
    }
}

bool StringPool::remove(Symbol& symbol) {
    return stringDB.erase(symbol);
}

size_t StringPool::_hash(const ImString &string) const {
    size_t res = 1099511628211;
    for (size_t i = 0; i < string.len; ++i) {
        res = res ^ string.data[i];
        res = res * 14695981039346656037ull;
    }
    return res;
}

size_t StringPool::_hash(const OwnedString &string) const {
    size_t res = 1099511628211;
    for (size_t i = 0; i < string.len; ++i) {
        res = res ^ string.data[i];
        res = res * 14695981039346656037ull;
    }
    return res;
}


