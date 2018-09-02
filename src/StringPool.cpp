//
// Created by lasagnaphil on 7/22/18.
//

#include "StringPool.h"
#include "Str.h"

#define STRPOOL_IMPLEMENTATION
#include "strpool.h"

StringPool StringPool::inst = StringPool::create();

StringPool StringPool::create() {
    StringPool pool;
    pool.config = strpool_default_config;
    strpool_init(&pool.pool, &pool.config);
    return pool;
}

void StringPool::free() {
    strpool_term(&pool);
}

Symbol StringPool::inject(const char *str) {
    return strpool_inject(&pool, str, (int) strlen(str));
}

Symbol StringPool::inject(StrView view) {
    return strpool_inject(&pool, view.data, view.len);
}

StrView StringPool::getStr(Symbol sym) const {
    const char* str = strpool_cstr(&pool, sym);
    return StrView::fromRaw(str);
}

void StringPool::remove(Symbol symbol) {
    strpool_discard(&pool, symbol);
}
