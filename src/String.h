//
// Created by lasagnaphil on 7/10/18.
//

#include <cstddef>
#include <cstring>

#ifndef THESYSTEM_STRING_H
#define THESYSTEM_STRING_H

#include "Map.h"
#include "StringPool.h"

#include <cstdio> /* needed for vsnprintf */
#include <cstdlib> /* needed for malloc-free */
#include <cstdarg> /* needed for va_list */

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
struct ImString {
    size_t len;
    const char* data;
    static ImString create(const char* str) {
        return ImString {
            .len = strlen(str),
            .data = str
        };
    }
    static ImString empty() {
        return ImString {
            .len = 0,
            .data = ""
        };
    }

    inline bool operator==(ImString other) const {
        if (other.len != len) return false;
        return strcmp(other.data, data) == 0;
    }
};

struct String {
    size_t len;
    char* data;
    static String create(char* str) {
        return String {
            .len = strlen(str),
            .data = str
        };
    }
    ImString toImmutable() {
        return ImString {
                .len = len,
                .data = data
        };
    }

    inline bool operator==(String other) const {
        if (other.len != len) return false;
        return strcmp(other.data, data) == 0;
    }
};

struct OwnedString {
    size_t len;
    char* data;
    static OwnedString create(const char* str) {
        size_t len = strlen(str);
        char* heapStr = new char[len + 1];
        strcpy(heapStr, str);
        return OwnedString {
            .len = len,
            .data = heapStr
        };
    }
    static OwnedString create(const char* str, size_t len) {
        char* heapStr = new char[len + 1];
        strcpy(heapStr, str);
        return OwnedString {
            .len = len,
            .data = heapStr
        };
    }
    static OwnedString create(size_t len) {
        char* heapStr = new char[len + 1];
        return OwnedString {
            .len = len,
            .data = heapStr
        };
    }
    static OwnedString fmt(const char* fmt, ...) {
        char** str;
        va_list ap;
        va_start(ap, fmt);
        int r = vasprintf_alt(str, fmt, ap);
        va_end(ap);
        return OwnedString {
            .len = strlen(*str),
            .data = *str
        };
    }
    void free() {
        delete[] data;
    }
};

#endif //THESYSTEM_STRING_H
