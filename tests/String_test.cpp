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
    SECTION("String find") {
        String str = "The quick brown fox jumps over the lazy dog";

        REQUIRE(str.find('o') == 12);
        REQUIRE(str.findReverse('o') == 41);
        {
            auto res = str.findAll('o');
            REQUIRE(res.size == 4);
            REQUIRE(res[0] == 12);
            REQUIRE(res[1] == 17);
            REQUIRE(res[2] == 26);
            REQUIRE(res[3] == 41);
            res.free();
        }

        REQUIRE(str.find(lit("brown")) == 10);
        REQUIRE(str.findReverse(lit("brown")) == 10);
        {
            auto res = str.findAll(lit("he"));
            REQUIRE(res.size == 2);
            REQUIRE(res[0] == 1);
            REQUIRE(res[1] == 32);
            res.free();
        }

        str.free();
    }
}

TEST_CASE("Symbol/StringPool works!", "[String]") {
    SECTION("Is POD") {
        REQUIRE(std::is_pod<StringPool>());
    }

    SECTION("Create and use symbols") {
        StringPool pool = StringPool::create();
        defer {pool.free();};
        Symbol sym1 = pool.inject("one");
        Symbol sym2 = pool.inject(lit("two"));
        Symbol sym3 = pool.inject(lit("three"));

        Symbol otherSym1 = pool.inject(lit("one"));
        Symbol otherSym2 = pool.inject(lit("two"));
        Symbol otherSym3 = pool.inject(lit("three"));
        REQUIRE(sym1 == otherSym1);
        REQUIRE(sym2 == otherSym2);
        REQUIRE(sym3 == otherSym3);
        REQUIRE(pool.getStr(sym1) == lit("one"));
        REQUIRE(pool.getStr(sym2) == lit("two"));
        REQUIRE(pool.getStr(sym3) == lit("three"));
    }
}

