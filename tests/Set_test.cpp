//
// Created by lasagnaphil on 8/10/18.
//
#include "catch.hpp"
#include "Set.h"
#include "Defer.h"

TEST_CASE("Set works", "[Set]") {
    SECTION("Is POD") {
        REQUIRE(std::is_pod<Set<int>>());
    }
    SECTION("create with default arg, insert, has, free") {
        auto set = Set<int>::create();
        defer {set.free();};

        for (int i = 0; i < 100; ++i) {
            set.insert(i);
        }
        REQUIRE(set.size() == 100);
        for (int i = 0; i < 100; ++i) {
            REQUIRE(set.has(i));
        }
        REQUIRE(!set.has(100));
    }
}
