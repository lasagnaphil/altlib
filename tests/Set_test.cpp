//
// Created by lasagnaphil on 8/10/18.
//
#include "doctest.h"
#include "Set.h"
#include "Defer.h"

TEST_CASE("Set works") {
    SUBCASE("Is POD") {
        REQUIRE(std::is_pod<Set<int>>());
    }
    SUBCASE("create with default arg, insert, has, free") {
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
    SUBCASE("insert duplicates") {
        auto set = Set<int>::create();
        defer {set.free();};

        for (int i = 0; i < 100; ++i) {
            set.insert(42);
        }

        REQUIRE(set.size() == 1);
        REQUIRE(set.has(42));
        REQUIRE(!set.has(43));
    }
    SUBCASE("insert and remove") {
        auto set = Set<int>::create();
        defer {set.free();};

        for (int i = 0; i < 100; ++i) {
            set.insert(i);
        }
        for (int i = 50; i < 100; ++i) {
            set.remove(i);
        }
        for (int i = 0; i < 50; ++i) {
            REQUIRE(set.has(i));
        }
        for (int i = 50; i < 100; ++i) {
            REQUIRE(!set.has(i));
        }
    }
}
