//
// Created by lasagnaphil on 7/14/18.
//

#ifndef THESYSTEM_OPTION_H
#define THESYSTEM_OPTION_H

#include <cassert>

template <typename T>
struct Option {
    T data;
    bool isSome;

    static Option<T> some(const T& data) {
        return Option {data, true};
    }
    static Option<T> none() {
        Option option;
        option.isSome = false;
        return option;
    }

    T unwrap() {
        assert(isSome);
        if (!isSome) {
            fprintf(stderr, "Error on unwrap [File %s, Line %d]", __FILE__, __LINE__);
            exit(1);
        }
        return data;
    }

    template <typename U, typename Fun>
    Option<U> map(Fun f) {
        return isSome? Option<U>::some(f(data)) : Option<U>::none();
    };

    T unwrap_or(T other) {
        return isSome? data : other;
    }
};

#endif //THESYSTEM_OPTION_H
