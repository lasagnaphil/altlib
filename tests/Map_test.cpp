//
// Created by lasagnaphil on 7/10/18.
//

#include "catch.hpp"
#include "Map.h"
#include "Defer.h"

TEST_CASE("Map works", "[Map]") {
    SECTION("Is POD") {
        REQUIRE(std::is_pod<Map<int, int>>());
    }
    SECTION("init, insert, get, tryGet, size") {
        auto map = Map<int, int>::create();
        defer {map.free();};
        map.insert(0, 1);
        map.insert(1, 1);
        map.insert(2, 2);
        map.insert(3, 3);
        map.insert(4, 5);
        REQUIRE(map.get(0) == 1);
        REQUIRE(map.get(1) == 1);
        REQUIRE(map.get(2) == 2);
        REQUIRE(map.get(3) == 3);
        REQUIRE(map.get(4) == 5);
        REQUIRE(*map.tryGet(4) == 5);
        REQUIRE(map.tryGet(5) == nullptr);
        REQUIRE(map.size() == 5);
    }

    SECTION("insert / set / erase a lot") {
        auto map = Map<int, int>::create();
        defer {map.free();};
        for (int i = 0; i < 100; ++i) {
            map.insert(i, i+1);
        }
        for (int i = 0; i < 100; ++i) {
            REQUIRE(map.get(i) == i+1);
        }
        REQUIRE(map.size() == 100);
        for (int i = 0; i < 50; ++i) {
            map.erase(2*i);
        }
        for (int i = 0; i < 50; ++i) {
            REQUIRE(map.get(2*i + 1) == 2*i + 2);
        }
        REQUIRE(map.size() == 50);
        for (int i = 0; i < 50; ++i) {
            map.set(2*i + 1, i);
        }
        for (int i = 0; i < 50; ++i) {
            REQUIRE(map.get(2*i + 1) == i);
        }
        REQUIRE(map.size() == 50);
    }

    SECTION("insert using index operator") {
        auto map = Map<int, int>::create();
        defer {map.free();};
        for (int i = 0; i < 8; ++i) {
            map[i] = i + 1;
        }
        for (int i = 0; i < 8; ++i) {
            REQUIRE(map[i] == i + 1);
        }
        REQUIRE(map.size() == 8);
    }
}