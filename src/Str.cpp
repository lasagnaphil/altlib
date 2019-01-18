//
// Created by lasagnaphil on 2018-11-13.
//

#include "Str.h"

#include "Result.h"

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
    char *str = new char[len + 1];
    int r = vsnprintf(str, len + 1, fmt, ap); /* "secure" version of vsprintf */
    if (r == -1) return delete[] str, -1;
    *strp = str;
    return r;
}

Res<String> String::fmt(const char *fmt, ...) {
    char* str;
    va_list ap;
    va_start(ap, fmt);
    int r = vasprintf_alt(&str, fmt, ap);
    va_end(ap);
    if (r != -1)
        return Res<String>::ok(String::create(lit(str)));
    else
        return Res<String>::err(Err::create(
                lit("Error during string formatting")));
}

String String::create(const char *str) {
    return create(lit(str));
}

String String::create(StrView str) {
    String string;
    string.buffer = Vec<char>::create(str.len + 1);
    memcpy(string.buffer.data, str.data, (str.len + 1) * sizeof(char));
    string.buffer.size = str.len + 1;
    string.len = str.len;
    return string;
}

void String::append(StrView str) {
    if (buffer.capacity < len + str.len + 1)
    {
        buffer.reserve(2 * buffer.capacity);
    }
    memcpy(buffer.data + str.len - 1, str.data, str.len);
    buffer[buffer.size - 1] = '\0';
    len += str.len;
}

void String::append(char c) {
    buffer[buffer.size - 1] = c;
    buffer.push('\0');
    len++;
}

bool String::fmtAppend(const char *fmt, ...) {
    char* str;
    va_list ap;
    va_start(ap, fmt);
    int r = vasprintf_alt(&str, fmt, ap);
    va_end(ap);
    append(lit(str));
    delete[] str;
    return r != -1;
}

int String::find(char c, int startIdx) {
    for (int i = startIdx; i < len; i++) {
        if (buffer[i] == c) { return i; }
    }
    return -1;
}

int String::findReverse(char c) { return findReverse(c, len - 1); }

int String::findReverse(char c, int startIdx) {
    for (int i = startIdx; i >= 0; i--) {
        if (buffer[i] == c) { return i; }
    }
    return -1;
}

Vec<int> String::findAll(char c, int startIdx) {
    Vec<int> vec = {};
    for (int i = startIdx; i < len; i++) {
        if (buffer[i] == c) { vec.push(i); }
    }
    return vec;
}

int String::find(StrView target, int startIdx) {
    for (int i = startIdx; i < len - target.len; i++) {
        int res = i;
        int j = 0;
        for (; j < target.len && buffer[i] == target.data[j]; i++, j++);
        if (j == target.len) { return res; }
    }
    return -1;
}

int String::findReverse(StrView target) { return findReverse(target, len - 1); }

int String::findReverse(StrView target, int startIdx) {
    for (int i = startIdx; i >= target.len - 1; i--) {
        int j = target.len - 1;
        for (; j >= 0 && buffer[i] == target.data[j]; i--, j--);
        if (j == -1) { return i + 1; }
    }
    return -1;
}

Vec<int> String::findAll(StrView target, int startIdx) {
    Vec<int> vec = {};
    for (int i = startIdx; i < len - target.len; i++) {
        int resIdx = i;
        int j = 0;
        for (; j < target.len && buffer[i] == target.data[j]; i++, j++);
        if (j == target.len) { vec.push(resIdx); }
    }
    return vec;
}
