//
// Created by lasagnaphil on 7/10/18.
//

#ifndef ALTLIB_STRING_H
#define ALTLIB_STRING_H

#include "Vec.h"

#include <stdio.h> /* needed for vsnprintf */
#include <stdlib.h> /* needed for malloc-free */
#include <stdarg.h> /* needed for va_list */
#include <memory.h>
#include <stddef.h>
#include <string.h>

static int _vscprintf_so_alt(const char * format, va_list pargs) {
    int retval;
    va_list argcopy;
    va_copy(argcopy, pargs);
    retval = vsnprintf(NULL, 0, format, argcopy);
    va_end(argcopy);
    return retval;
}

static int vasprintf_alt(char **strp, const char *fmt, va_list ap) {
    int len = _vscprintf_so_alt(fmt, ap);
    if (len == -1) return -1;
    char *str = new char[(size_t) len + 1];
    int r = vsnprintf(str, len + 1, fmt, ap); /* "secure" version of vsprintf */
    if (r == -1) return free(str), -1;
    *strp = str;
    return r;
}

struct StrView {
    const char* data;
    size_t len;

    static StrView fromRaw(const char* str) {
        return fromRaw(str, strlen(str));
    }

    static StrView fromRaw(const char* str, size_t len) {
        return StrView {
            str,
            len
        };
    }

    static const StrView empty() {
        return StrView::fromRaw("");
    }

    inline bool operator==(StrView other) const {
        if (other.len != len) return false;
        return memcmp(other.data, data, len) == 0;
    }
};


struct String {
    Vec<char> buffer;

    static String create(const char* str) {
        return String::create(str, strlen(str));
    }

    static String create(const char* str, size_t len) {
        String string;
        string.buffer = Vec<char>::create(len + 1);
        memcpy(string.buffer.data, str, (len + 1) * sizeof(char));
        string.buffer.size = len + 1;
        return string;
    }

    static String create(size_t len) {
        String string;
        string.buffer = Vec<char>::create(len + 1);
        return string;
    }

    static String fmt(const char* fmt, ...) {
        char* str;
        va_list ap;
        va_start(ap, fmt);
        int r = vasprintf_alt(&str, fmt, ap);
        va_end(ap);
        return String::create(str);
    }

    void free() {
        buffer.free();
    }

    char* data() const {
        return buffer.data;
    }

    size_t len() const {
        return buffer.size - 1;
    }

    StrView getView() const {
        return StrView::fromRaw(buffer.data, buffer.size - 1);
    }

    inline bool operator==(String other) const {
        if (other.buffer.size != buffer.size) return false;
        return memcmp(other.buffer.data, buffer.data, buffer.size) == 0;
    }
};

#endif //ALTLIB_STRING_H
