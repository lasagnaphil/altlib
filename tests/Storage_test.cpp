//
// Created by lasagnaphil on 7/14/18.
//

#include "doctest.h"
#include "Storage.h"
#include "Defer.h"

struct TestRes {
    int a;
    double b;
    int c;

    inline bool operator==(TestRes other) const {
        return other.a == a && other.b == b && other.c == c;
    }
};
template <> uint16_t TypeRegistry::getID<TestRes>() { return 1; }

TEST_CASE("Storage works") {
    SUBCASE("Is POD") {
        REQUIRE(std::is_pod<Storage<TestRes>>());
    }
    SUBCASE("Storage creation") {
        auto storage = Storage<TestRes>::create(8);
        defer {storage.free();};
        REQUIRE(storage.capacity == 8);
    }
    SUBCASE("Storage test (small)") {
        auto storage = Storage<TestRes>::create(8);
        defer {storage.free();};
        // auto strpool = StringPool::create();
        // defer {strpool.free();};
        Ref<TestRes> ref1, ref2, ref3;
        REQUIRE(storage.count == 0);
        auto res1 = storage.insert(TestRes{1, 1.0, 1}, &ref1);
        REQUIRE(storage.count == 1);
        auto res2 = storage.insert(TestRes{2, 2.0, 2}, &ref2);
        REQUIRE(storage.count == 2);
        auto res3 = storage.insert(TestRes{3, 3.0, 3}, &ref3);
        REQUIRE(storage.count == 3);
        REQUIRE(storage.get(ref1) == TestRes{1, 1.0, 1});
        REQUIRE(storage.get(ref2) == TestRes{2, 2.0, 2});
        REQUIRE(storage.get(ref3) == TestRes{3, 3.0, 3});
        /*
        TestRes* getByNameRes1 = storage.getByName(strpool.getSym("one"));
        TestRes* getByNameRes2 = storage.getByName(strpool.getSym("two"));
        TestRes* getByNameRes3 = storage.getByName(strpool.getSym("three"));
        REQUIRE(getByNameRes1);
        REQUIRE(getByNameRes2);
        REQUIRE(getByNameRes3);
        REQUIRE(*getByNameRes1 == TestRes{1, 1.0, 1});
        REQUIRE(*getByNameRes2 == TestRes{2, 2.0, 2});
        REQUIRE(*getByNameRes3 == TestRes{3, 3.0, 3});
        REQUIRE(storage.getByName(strpool.getSym("four")) == nullptr);
        */
        REQUIRE(storage.has(ref1));
        REQUIRE(storage.has(ref2));
        REQUIRE(storage.has(ref3));
    }
    SUBCASE("Storage test (big, expanding)") {
#define TEST_SIZE 2048
        auto storage = Storage<TestRes>::create(1);
        defer {storage.free();};
        // auto strpool = StringPool::create();
        // defer {strpool.free();};

        // char str[TEST_SIZE][5];
        // Symbol symbols[TEST_SIZE];
        Ref<TestRes> refs[TEST_SIZE];
        for (int i = 0; i < TEST_SIZE; ++i) {
            // sprintf(str[i], "%d", i);
            // symbols[i] = strpool.add(str[i]);
            storage.insert(TestRes{i, (double)i, i}, &refs[i]);
        }
        REQUIRE(storage.count == TEST_SIZE);

        int indices[TEST_SIZE];
        for (int i = 0; i < TEST_SIZE; ++i) {
            indices[i] = i;
        }
        for (int i = TEST_SIZE-1; i >= 1; --i) {
            int j = rand() % i;
            int temp = indices[j];
            indices[j] = indices[i];
            indices[i] = temp;
        }
        for (int i = 0; i < TEST_SIZE/2; ++i) {
            storage.release(refs[indices[i]]);
        }
        for (int i = 0; i < TEST_SIZE/2; ++i) {
            TestRes* res = storage.tryGet(refs[indices[i]]);
            REQUIRE(res == nullptr);
        }
        for (int i = TEST_SIZE/2; i < TEST_SIZE; ++i) {
            TestRes res = storage.get(refs[indices[i]]);
            REQUIRE(res.a == indices[i]);
            REQUIRE(res.b == (double)indices[i]);
            REQUIRE(res.c == indices[i]);
        }
        REQUIRE(storage.count == TEST_SIZE/2);

        for (int i = 0; i < TEST_SIZE/2; ++i){
            storage.insert(TestRes{i, (double)i, i});
        }
        REQUIRE(storage.count == TEST_SIZE);

#undef TEST_SIZE
    }
}
