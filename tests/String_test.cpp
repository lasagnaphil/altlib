//
// Created by lasagnaphil on 7/14/18.
//

#include "catch.hpp"
#include "Defer.h"
#include "StringPool.h"

TEST_CASE("String works!", "[String]") {
    SECTION("Is POD") {
        REQUIRE(std::is_pod<String>());
    }
}

TEST_CASE("Symbol/StringPool works!", "[String]") {
    SECTION("Is POD") {
        REQUIRE(std::is_pod<StringPool>());
    }
    SECTION("Create and use symbols") {
        StringPool pool = StringPool::create();
        defer {pool.free();};
        Symbol sym1 = pool.inject(StrView::fromRaw("one"));
        Symbol sym2 = pool.inject("two");
        Symbol sym3 = pool.inject("three");

        Symbol otherSym1 = pool.inject(StrView::fromRaw("one"));
        Symbol otherSym2 = pool.inject(StrView::fromRaw("two"));
        Symbol otherSym3 = pool.inject(StrView::fromRaw("three"));
        REQUIRE(sym1 == otherSym1);
        REQUIRE(sym2 == otherSym2);
        REQUIRE(sym3 == otherSym3);
        REQUIRE(pool.getStr(sym1) == StrView::fromRaw("one"));
        REQUIRE(pool.getStr(sym2) == StrView::fromRaw("two"));
        REQUIRE(pool.getStr(sym3) == StrView::fromRaw("three"));
    }
}

