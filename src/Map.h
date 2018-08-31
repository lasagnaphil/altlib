//
// Created by lasagnaphil on 6/22/18.
//

#ifndef ALTLIB_MAP_H
#define ALTLIB_MAP_H

#include <limits.h>
#include <stdint.h>
#include "xxhash.h"
#include "Str.h"

template <typename Key>
struct HashFn {
    size_t operator()(const Key& key) const;
};

#define DEFAULT_HASHFN(__Key) \
template <> \
struct HashFn<__Key> { \
    size_t operator()(const __Key& key) const { \
        return XXH64((void*) &key, sizeof(__Key), 0); \
    }\
};

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
        int64_t probeLength;
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

    int64_t _maxProbeLength;

    HashFn<K> _hashFn;

    static Map create() {
        Map map;
        map._size = 0;
        map._bucketCount = 0;
        map._buckets = nullptr;
        map._states = nullptr;
        map._maxProbeLength = -1;
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
        memset(_buckets, 0, sizeof(Bucket) * newBucketCount);
        memset(_states, 0, sizeof(State) * newBucketCount);
        _size = 0;
        _maxProbeLength = -1;

        for (size_t i = 0; i < _bucketCount; ++i) {
            _states[i] = State::INACTIVE;
        }

        for (size_t i = 0; i < oldBucketCount; i++) {
            if (oldStates[i] == State::FILLED) {
                insert(oldBuckets[i].key, oldBuckets[i].value);
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
        _maxProbeLength = -1;
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

    bool contains(const K& key) const {
        return _findFilledBucket(key) != (size_t)-1;
    }

    void set(const K& key, const V& value) {
        size_t i = _findFilledBucket(key);
        assert(_states[i] == State::FILLED);
        _buckets[i].value = value;
    }

    void insert(K key, V value) {
        assert(!contains(key));
        _checkExpandNeed();
        size_t hash = _hashFn(key);
        size_t start = hash % _bucketCount;
        size_t i;
        for (int64_t offset = 0; offset <= _bucketCount; ++offset) {
            i = (start + offset) % _bucketCount;
            if (_states[i] != State::FILLED) {
                if (offset > _maxProbeLength) {
                    _maxProbeLength = offset;
                }
                _states[i] = State::FILLED;
                _buckets[i] = Bucket{key, offset, hash, value};
                _size++;
                return;
            }
            if (offset > _buckets[i].probeLength) {
                if (_states[i] == State::ACTIVE) {
                    if (offset > _maxProbeLength) {
                        _maxProbeLength = offset;
                    }
                    _states[i] = State::FILLED;
                    _buckets[i] = Bucket{key, offset, hash, value};
                    _size++;
                    return;
                }
                Bucket tempBucket = _buckets[i];
                _states[i] = State::FILLED;
                _buckets[i] = Bucket{key, offset, hash, value};
                key = tempBucket.key;
                offset = tempBucket.probeLength;
                hash = tempBucket.hash;
                value = tempBucket.value;
            }
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

    size_t _findFilledBucket(const K& key) const {
        if (empty()) { return (size_t)-1; }
        size_t hash = _hashFn(key);
        size_t start = hash % _bucketCount;
        size_t i;
        for (int64_t offset = 0; offset <= _maxProbeLength; ++offset) {
            i = (start + offset) % _bucketCount;
            if (_states[i] == State::INACTIVE) {
                return (size_t)-1;
            }
            else if (offset > _buckets[i].probeLength) {
                return (size_t)-1;
            }
            else if (_states[i] == State::FILLED) {
                if (_buckets[i].hash == hash && _buckets[i].key == key) {
                    return i;
                }
            }
        }
        return (size_t)-1;
    }

    void _checkExpandNeed() {
        reserve(_size + 1);
    }
};

template <>
struct HashFn<String> {
    size_t operator()(const String& string) const {
        return XXH64(string.buffer.data, string.buffer.size - 1, 0);
    }
};

template <>
struct HashFn<StrView> {
    size_t operator()(const StrView& strView) const {
        return XXH64(strView.data, strView.len, 0);
    }
};

template <>
struct HashFn<Symbol> {
    size_t operator()(const Symbol& symbol) const {
        return XXH64(&symbol.index, sizeof(symbol.index), 0);
    }
};

/// Explicit specialization for bool.
DEFAULT_HASHFN(bool)

/// Explicit specialization for char.
DEFAULT_HASHFN(char)

/// Explicit specialization for signed char.
DEFAULT_HASHFN(signed char)

/// Explicit specialization for unsigned char.
DEFAULT_HASHFN(unsigned char)

/// Explicit specialization for wchar_t.
DEFAULT_HASHFN(wchar_t)

/// Explicit specialization for char16_t.
DEFAULT_HASHFN(char16_t)

/// Explicit specialization for char32_t.
DEFAULT_HASHFN(char32_t)

/// Explicit specialization for short.
DEFAULT_HASHFN(short)

/// Explicit specialization for int.
DEFAULT_HASHFN(int)

/// Explicit specialization for long.
DEFAULT_HASHFN(long)

/// Explicit specialization for long long.
DEFAULT_HASHFN(long long)

/// Explicit specialization for unsigned short.
DEFAULT_HASHFN(unsigned short)

/// Explicit specialization for unsigned int.
DEFAULT_HASHFN(unsigned int)

/// Explicit specialization for unsigned long.
DEFAULT_HASHFN(unsigned long)

/// Explicit specialization for unsigned long long.
DEFAULT_HASHFN(unsigned long long)

#endif //ALTLIB_MAP_H
