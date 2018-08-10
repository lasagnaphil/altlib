//
// Created by lasagnaphil on 7/14/18.
//

#include "catch.hpp"
#include "Defer.h"
#include "String.h"
#include "Symbol.h"

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
        Symbol sym1 = pool.add(ImString::create("one"));
        Symbol sym2 = pool.add("two");
        Symbol sym3 = pool.add("three");

        Symbol otherSym1 = pool.getSym(ImString::create("one"));
        Symbol otherSym2 = pool.getSym(ImString::create("two"));
        Symbol otherSym3 = pool.getSym(ImString::create("three"));
        REQUIRE(sym1 == otherSym1);
        REQUIRE(sym2 == otherSym2);
        REQUIRE(sym3 == otherSym3);
        REQUIRE(pool.getString(sym1) == ImString::create("one"));
        REQUIRE(pool.getString(sym2) == ImString::create("two"));
        REQUIRE(pool.getString(sym3) == ImString::create("three"));
    }
}

