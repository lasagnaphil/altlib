//
// Created by lasagnaphil on 7/10/18.
//

#include "doctest.h"
#include "Vec.h"
#include "Defer.h"

TEST_CASE("Vec works") {
    SUBCASE("Is POD") {
        REQUIRE(std::is_pod<Vec<int>>());
    }
    SUBCASE("create with default arg, push, free") {
        Vec<int> vec = {};
        defer {vec.free();};

        for (int i = 0; i < 100; ++i) {
            vec.push(i);
        }
        REQUIRE(vec.size == 100);
        REQUIRE(vec.capacity == 128);
    }
    SUBCASE("create with initializer list") {
        {
            Vec<int> vec {0, 1, 2, 3};
            defer {vec.free();};
            REQUIRE(vec.size == 4);
            REQUIRE(vec.capacity == 4);
            REQUIRE(vec[0] == 0);
            REQUIRE(vec[1] == 1);
            REQUIRE(vec[2] == 2);
            REQUIRE(vec[3] == 3);
        }
        {
            Vec<int> vec = {0, 1, 2, 3};
            defer {vec.free();};
            REQUIRE(vec.size == 4);
            REQUIRE(vec.capacity == 4);
            REQUIRE(vec[0] == 0);
            REQUIRE(vec[1] == 1);
            REQUIRE(vec[2] == 2);
            REQUIRE(vec[3] == 3);
        }
    }
    SUBCASE("create with capacity, push, pop, free") {
        auto vec = Vec<int>::create(2);
        defer {vec.free();};

        vec.push(0);
        vec.push(1);
        vec.push(3);
        vec.pop();
        vec.push(5);
        REQUIRE(vec.size == 3);
        REQUIRE(vec.capacity == 4);
        REQUIRE(vec[0] == 0);
        REQUIRE(vec[1] == 1);
        REQUIRE(vec[2] == 5);
    }
    SUBCASE("create from duplication") {
        auto vec = Vec<int>::fromElem(42, 3);
        defer {vec.free();};

        REQUIRE(vec[0] == 42);
        REQUIRE(vec[1] == 42);
        REQUIRE(vec[2] == 42);
    }
    SUBCASE("clone") {
        auto vec = Vec<int>::create(4);
        defer {vec.free();};
        vec.push(0);
        vec.push(1);
        vec.push(2);

        auto otherVec = vec.clone();
        defer {otherVec.free();};
        REQUIRE(vec[0] == otherVec[0]);
        REQUIRE(vec[1] == otherVec[1]);
        REQUIRE(vec[2] == otherVec[2]);
    }
    SUBCASE("expand, shrink") {
        auto vec = Vec<int>::create(3);
        defer {vec.free();};

        vec.push(0);
        vec.push(1);
        vec.push(2);
        vec.reserve(5);
        REQUIRE(vec.size == 3);
        REQUIRE(vec.capacity == 5);
        vec.shrink(2);
        REQUIRE(vec.size == 2);
        REQUIRE(vec.capacity == 2);
        REQUIRE(vec[0] == 0);
        REQUIRE(vec[1] == 1);
    }
}
