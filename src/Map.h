//
// Created by lasagnaphil on 6/22/18.
//

#ifndef ALTLIB_MAP_H
#define ALTLIB_MAP_H

#include <climits>
#include <cstdint>
#include "Vec.h"

static inline size_t rotl32(size_t n, unsigned int c) {
    const unsigned int mask = (CHAR_BIT * sizeof(n) - 1);
    c &= mask;
    return (n<<c) | (n>>( (-c)&mask));
}

static inline size_t rotr32(size_t n, unsigned int c) {
    const unsigned int mask = (CHAR_BIT * sizeof(n) - 1);
    c &= mask;
    return (n>>c) | (n<<( (-c)&mask));
}

#define MAP_FOREACH(__map, __key, __value, __CODE) \
for (int __i = 0; __i < __map._bucketCount; __i++) { \
    if ((uint8_t)(__map._states[__i]) == 2) { \
        auto& __key = __map._buckets[__i].key; \
        auto& __value = __map._buckets[__i].value; \
        __CODE \
    } \
}

// Courtesy of emilib (https://github.com/emilk/emilib), which a lot of this code is based on

// NOTE: K, V must be POD (no non-trivial constructor / destructor)
template <typename K, typename V>
struct Map {
    struct Bucket {
        K key;
        size_t hash;
        V value;
    };

    enum class State : uint8_t {
        INACTIVE, ACTIVE, FILLED
    };

    Bucket* _buckets;
    State* _states;
    size_t _size;
    size_t _bucketCount;

    static Map create() {
        Map map;
        map._size = 0;
        map._bucketCount = 0;
        map._buckets = nullptr;
        map._states = nullptr;
        return map;
    }

    void free() {
        delete[] _buckets;
        delete[] _states;
    }

    void reserve(size_t numElems) {
        size_t requiredBuckets = numElems + numElems/2 + 1;
        if (requiredBuckets <= _bucketCount) { return; }
        size_t newBucketCount = 4;
        while (newBucketCount < requiredBuckets) { newBucketCount *= 2; }

        size_t oldBucketCount = _bucketCount;
        Bucket* oldBuckets = _buckets;
        State* oldStates = _states;

        _bucketCount = newBucketCount;
        _buckets = new Bucket[newBucketCount];
        _states = new State[newBucketCount];

        for (size_t i = 0; i < _bucketCount; ++i) {
            _states[i] = State::INACTIVE;
        }

        for (size_t i = 0; i < oldBucketCount; i++) {
            if (oldStates[i] == State::FILLED) {
                auto srcBucket = oldBuckets[i];
                auto dstBucketIdx = _findEmptyBucket(srcBucket.key);
                _buckets[dstBucketIdx] = srcBucket;
                _buckets[dstBucketIdx].hash = dstBucketIdx;
                _states[dstBucketIdx] = State::FILLED;
            }
        }

        if (oldBucketCount > 0) {
            delete[] oldBuckets;
            delete[] oldStates;
        }
    }

    void clear() {
        for (size_t i = 0; i < _bucketCount; ++_bucketCount) {
            if (_states[i] == State::FILLED) {
                _states[i] = State::INACTIVE;
            }
        }
        _size = 0;
    }


    bool empty() const { return _size == 0; };

    size_t size() const { return _size; }

    V& get(const K& key) const {
        size_t i = _findFilledBucket(key);
        assert(i != (size_t)(-1) && "key not found");
        V& value = _buckets[i].value;
        return value;
    }

    V* tryGet(const K& key) const {
        size_t i = _findFilledBucket(key);
        if (i == (size_t)-1) {
            return nullptr;
        }
        return &_buckets[i].value;
    }

    V& operator[](const K& key) {
        _checkExpandNeed();
        auto i = _findOrAllocate(key);
        if (_states[i] != State::FILLED) {
            _states[i] = State::FILLED;
            _buckets[i] = Bucket {.key = key, .hash = i};
            _size++;
        }
        return _buckets[i].value;
    }

    bool contains(const K& key) const {
        return _findFilledBucket(key) != (size_t)-1;
    }

    void set(const K& key, const V& value) {
        size_t i = _findFilledBucket(key);
        assert(_states[i] == State::FILLED);
        _buckets[i].value = value;
    }

    void insert(const K& key, const V& value) {
        assert(!contains(key));
        _checkExpandNeed();
        size_t i = _findEmptyBucket(key);
        _states[i] = State::FILLED;
        _buckets[i] = Bucket{.key = key, .hash = i, .value = value};
        _size++;
    }

    void insertOrAssign(const K& key, const V& value) {
        size_t i = _findFilledBucket(key);
        if (_states[i] == State::FILLED) {
            _buckets[i].value = value;
        }
        else {
            _checkExpandNeed();
            _states[i] = State::FILLED;
            _buckets[i] = Bucket{.key = key, .hash = i, .value = value};
            _size++;
        }
    }

    bool erase(const K& key) {
        size_t i = _findFilledBucket(key);
        if (i != (size_t) -1) {
            _states[i] = State::ACTIVE;
            _size--;
            return true;
        }
        else {
            return false;
        }
    }


    inline size_t _hashCombine(size_t hash, size_t i) const {
        return (rotl32(hash, 5) ^ i) * hash;
    }

    inline size_t _hash(const K& key) const {
        auto bytes = reinterpret_cast<const unsigned char*>(&key);
        size_t hash = 0x517cc1b727220a95;
        for (size_t i = 0; i < sizeof(key); ++i) {
            hash = _hashCombine(hash, bytes[i]);
        }
        return hash;
    }

    size_t _findEmptyBucket(const K& key) {
        size_t start = _hash(key) % _bucketCount;
        size_t i;
        for (size_t offset = 0; offset < _bucketCount; ++offset) {
            i = (start + offset) % _bucketCount;
            if (_states[i] != State::FILLED) {
                return i;
            }
        }
        assert("could not find empty bucket");
        return (size_t)-1;
    }

    size_t _findFilledBucket(const K& key) const {
        if (empty()) { return (size_t)-1; }
        size_t start = _hash(key) % _bucketCount;
        size_t i;
        for (size_t offset = 0; offset < _bucketCount; ++offset) {
            i = (start + offset) % _bucketCount;
            if (_states[i] == State::FILLED) {
                if (_buckets[i].key == key) {
                    return i;
                }
            }
            else if (_states[i] == State::INACTIVE) {
                return (size_t)-1;
            }
        }
        return (size_t)-1;
    }

    size_t _findOrAllocate(const K& key) {
        size_t start = _hash(key) % _bucketCount;
        size_t hole = (size_t)-1;
        size_t i;
        bool foundHole = false;
        size_t offset = 0;
        for (; offset < _bucketCount; ++offset) {
            i = (start + offset) % _bucketCount;
            if (_states[i] == State::FILLED) {
                if (_buckets[i].key == key) {
                    return i;
                }
            }
            else if (_states[i] == State::INACTIVE) {
                if (hole == (size_t)-1 && !foundHole) {
                    hole = i;
                    foundHole = true;
                }
            }
        }
        assert(offset == _bucketCount);
        assert(hole != (size_t)-1);

        return hole;
    }

    void _checkExpandNeed() {
        reserve(_size + 1);
    }
};


#endif //ALTLIB_MAP_H
