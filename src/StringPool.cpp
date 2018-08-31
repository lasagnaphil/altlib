//
// Created by lasagnaphil on 7/22/18.
//

#include "StringPool.h"
#include "Str.h"

StringPool StringPool::inst = StringPool::create();

StringPool StringPool::create() {
    StringPool pool;
    pool.stringDB = Map<Symbol, StrView>::create();
    pool.allocStrs = Vec<String>::create(16);
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
    StrView view = StrView::fromRaw(str);
    return add(view);
}

Symbol StringPool::add(StrView view) {
    Symbol symbol {_hash(view)};
    stringDB.insert(symbol, view);
    return symbol;
}

Symbol StringPool::heapAdd(const char *str) {
    auto string = String::create(str);
    allocStrs.push(string);
    Symbol symbol {_hash(string)};
    stringDB.insert(symbol, string.getView());
    return symbol;
}

Symbol StringPool::heapAdd(String string) {
    allocStrs.push(string);
    Symbol symbol {_hash(string)};
    stringDB.insert(symbol, string.getView());
    return symbol;
}

Symbol StringPool::getSym(StrView view) const {
    return Symbol {_hash(view)};
}

Symbol StringPool::getSym(const char *str) const {
    StrView view = StrView::fromRaw(str);
    return Symbol {_hash(view)};
}

StrView StringPool::getStr(Symbol &sym) const {
    if (sym.index > 0) {
        return stringDB.get(sym);
    }
    else {
        return StrView::empty();
    }
}

bool StringPool::remove(Symbol& symbol) {
    return stringDB.erase(symbol);
}

size_t StringPool::_hash(StrView view) const {
    size_t res = 1099511628211;
    for (size_t i = 0; i < view.len; ++i) {
        res = res ^ view.data[i];
        res = res * 14695981039346656037ull;
    }
    return res;
}

size_t StringPool::_hash(String string) const {
    size_t res = 1099511628211;
    for (size_t i = 0; i < string.buffer.size - 1; ++i) {
        res = res ^ string.buffer.data[i];
        res = res * 14695981039346656037ull;
    }
    return res;
}
