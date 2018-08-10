//
// Created by lasagnaphil on 7/10/18.
//

#include "catch.hpp"
#include "StackVec.h"

TEST_CASE("StackVec works", "[StackVec]") {
    SECTION("Is POD") {
        REQUIRE(std::is_pod<StackVec<int, 16>>());
    }
    SECTION("create with default arg, push, free") {
        auto vec = StackVec<int, 4>::create();
        vec.push(0);
        vec.push(1);
        vec.push(3);
        vec.push(5);
        REQUIRE(vec._size == 4);
    }
    SECTION("create with capacity, push, pop, free") {
        auto vec = StackVec<int, 4>();
        vec.push(0);
        vec.push(1);
        vec.push(3);
        vec.pop();
        vec.push(5);
        REQUIRE(vec._size == 3);
        REQUIRE(vec[0] == 0);
        REQUIRE(vec[1] == 1);
        REQUIRE(vec[2] == 5);
    }
    SECTION("create from duplication") {
        auto vec = StackVec<int, 4>::from(42, 3);

        REQUIRE(vec[0] == 42);
        REQUIRE(vec[1] == 42);
        REQUIRE(vec[2] == 42);
    }
    SECTION("clone") {
        auto vec = StackVec<int, 4>::create();
        vec.push(0);
        vec.push(1);
        vec.push(2);
        auto otherVec = vec.clone();
        REQUIRE(vec[0] == otherVec[0]);
        REQUIRE(vec[1] == otherVec[1]);
        REQUIRE(vec[2] == otherVec[2]);
    }
}