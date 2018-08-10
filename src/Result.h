//
// Created by lasagnaphil on 2018-07-23.
//

#ifndef ALTLIB_RESULT_H
#define ALTLIB_RESULT_H

#include <cstdio>
#include <cstdlib>
#include "log.h"
#include "String.h"

struct Err {
    OwnedString str;
    static Err create(const char* message) {
        Err err;
        err.str = OwnedString::create(message);
        return err;
    }
    void free() {
        str.free();
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

    static Result err(const char* errorMsg) {
        Result result;
        result.error = Error::create(errorMsg);
        result.isValid = false;
        return result;
    }

    Value unwrap() {
        assert(isValid);
        if (isValid) return value;
        else {
            log_error(error.str.data);
            exit(1);
        }
    }

    ~Result() {
        if (!isValid) { error.free(); }
    }
};
#endif //ALTLIB_RESULT_H
