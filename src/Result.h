//
// Created by lasagnaphil on 2018-07-23.
//

#ifndef ALTLIB_RESULT_H
#define ALTLIB_RESULT_H

#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "Str.h"

struct Ok {};

struct Err {
    String msg;
    static Err create(const char* message) {
        Err err;
        err.msg = String::create(message);
        return err;
    }
    static Err create(String string) {
        Err err;
        err.msg = string;
        return err;
    }

    void consume() {
        log_error(msg.buffer.data);
        msg.free();
        exit(EXIT_FAILURE);
    }

    void free() {
        msg.free();
    }
};

template <typename Value, typename Error>
struct Result {
    union {
        Value value;
        Error error;
    };
    bool isValid = false;

    static Result ok(Value value) {
        Result result;
        result.value = value;
        result.isValid = true;
        return result;
    }

    static Result err(Error error) {
        Result result;
        result.error = error;
        result.isValid = false;
        return result;
    }

    static Result err(const char* str) {
        Result result;
        result.error = Error::create(str);
        result.isValid = false;
        return result;
    }
    static Result err(String string) {
        Result result;
        result.error = Error::create(string);
        result.isValid = false;
        return result;
    }

    Value unwrap() {
        if (isValid) return value;
        else {
            error.consume();
            return value; // actually not called
        }
    }

    void free() {
        if (!isValid) { error.free(); }
    }
};

#endif //ALTLIB_RESULT_H
