//
// Created by lasagnaphil on 10/11/18.
//

#include "doctest.h"

#include "Queue.h"
#include "Defer.h"

TEST_CASE("Queue works") {
    SUBCASE("Is POD") {
        REQUIRE(std::is_pod<Queue<int>>());
    }

    SUBCASE("Enqueue and Dequeue works") {
        auto q = Queue<int>::create();
        defer {q.free();};

        q.enq(1);
        REQUIRE(q.size == 1);
        q.enq(2);
        REQUIRE(q.size == 2);
        q.enq(3);
        REQUIRE(q.size == 3);
        REQUIRE(q.deq() == 1);
        REQUIRE(q.size == 2);
        REQUIRE(q.deq() == 2);
        REQUIRE(q.size == 1);
        q.enq(4);
        REQUIRE(q.size == 2);
        q.enq(5);
        REQUIRE(q.size == 3);
        REQUIRE(q.deq() == 3);
        REQUIRE(q.size == 2);
    }

    SUBCASE("More Enqueue / Dequeue") {
        auto q = Queue<int>::create();
        defer {q.free();};

        for (int i = 0; i < 100; i++) {
            q.enq(i);
        }

        for (int i = 0; i < 50; i++) {
            REQUIRE(i == q.deq());
        }

        for (int i = 100; i < 150; i++) {
            q.enq(i);
        }

        for (int i = 50; i < 150; i++) {
            REQUIRE(i == q.deq());
        }
    }
}
