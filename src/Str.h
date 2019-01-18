//
// Created by lasagnaphil on 7/10/18.
//

#ifndef ALTLIB_STRING_H
#define ALTLIB_STRING_H

#include "Vec.h"
#include "Option.h"

#include <stdio.h> /* needed for vsnprintf */
#include <stdlib.h> /* needed for malloc-free */
#include <stdarg.h> /* needed for va_list */
#include <Mem.h>
#include <stddef.h>
#include <string.h>

#define lit(__STR) StrView::fromRaw(__STR)

struct StrView {
    const char* data;
    size_t len;

    static StrView fromRaw(const char* str) {
        return StrView(str);
    }

    static StrView fromRaw(const char* str, size_t len) {
        return StrView(str, len);
    }

    static StrView empty() {
        return StrView::fromRaw("");
    }

    StrView() = default;
    explicit StrView(const char* str) {
        data = str;
        len = strlen(str);
    }
    StrView(const char* str, size_t len) {
        data = str;
        this->len = len;
    }

    const char& operator[](size_t idx) const {
        assert(idx >= 0 && idx < len);
        return data[idx];
    }
};

template <typename T, typename U>
struct Result;

struct Err;

struct String {
    Vec<char> buffer;
    size_t len;

    static String create(const char* str);

    static String create(StrView str);

    static String create(size_t len) {
        String string;
        string.buffer = Vec<char>::create(len + 1);
        string.len = len;
        return string;
    }

    static Result<String, Err> fmt(const char* fmt, ...);

    String() = default;
    String(const char* str) {
        len = strlen(str);
        buffer = Vec<char>::create(len + 1);
        buffer.size = len + 1;
        memcpy(buffer.data, str, (len + 1) * sizeof(char));
    }

    void free() {
        buffer.free();
    }

    char* data() const {
        return buffer.data;
    }

    size_t size() const {
        return len;
    }

    size_t capacity() const {
        return buffer.size;
    }

    StrView getView() const {
        return StrView::fromRaw(buffer.data, len);
    }

	void append(StrView str);
	void append(char c);

	bool fmtAppend(const char* fmt, ...);

    int find(char c, int startIdx = 0);

    int findReverse(char c);
    int findReverse(char c, int startIdx);

    Vec<int> findAll(char c, int startIdx = 0);

    int find(StrView target, int startIdx = 0);

    int findReverse(StrView target);
    int findReverse(StrView target, int startIdx);

    Vec<int> findAll(StrView target, int startIdx = 0);
};

inline bool operator==(StrView lhs, StrView rhs) {
    if (lhs.len != rhs.len) return false;
    return memcmp(lhs.data, rhs.data, lhs.len) == 0;
}

inline bool operator!=(StrView lhs, StrView rhs) {
    if (lhs.len != rhs.len) return true;
    return memcmp(lhs.data, rhs.data, lhs.len) != 0;
}

inline bool operator==(String lhs, StrView rhs) {
    if (lhs.size() == rhs.len) return false;
    return memcmp(lhs.data(), rhs.data, lhs.size()) == 0;
}

inline bool operator!=(String lhs, StrView rhs) {
    if (lhs.size() == rhs.len) return true;
    return memcmp(lhs.data(), rhs.data, lhs.size()) != 0;
}

inline bool operator==(StrView lhs, String rhs) {
    if (lhs.len == rhs.size()) return false;
    return memcmp(lhs.data, rhs.data(), lhs.len) == 0;
}

inline bool operator!=(StrView lhs, String rhs) {
    if (lhs.len == rhs.size()) return true;
    return memcmp(lhs.data, rhs.data(), lhs.len) != 0;
}

inline bool operator==(String lhs, String rhs) {
    if (lhs.buffer.size != rhs.buffer.size) return false;
    return memcmp(lhs.buffer.data, rhs.buffer.data, lhs.buffer.size) == 0;
}

inline bool operator!=(String lhs, String rhs) {
    if (lhs.buffer.size != rhs.buffer.size) return true;
    return memcmp(lhs.buffer.data, rhs.buffer.data, lhs.buffer.size) != 0;
}

#endif //ALTLIB_STRING_H
