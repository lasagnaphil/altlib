//
// Created by lasagnaphil on 7/22/18.
//

#ifndef ALTLIB_FILE_H
#define ALTLIB_FILE_H

#include <cstdio>
#include <fcntl.h>
#include <cerrno>
#include "Option.h"
#include "String.h"
#include "Result.h"

struct File {
    FILE* file;

    static Result<File, Err> open(const char* filename, const char* permissions) {
        File file;
        file.file = fopen(filename, permissions);
        if (file.file == NULL) {
            return Result<File, Err>::err(strerror(errno));
        }
        return Result<File, Err>::ok(file);
    }

    Result<OwnedString, Err> readAll() {
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, 0, SEEK_SET);
        OwnedString str = OwnedString::create("", size);
        if (size != fread(str.data, sizeof(char), size, file)) {
            str.free();
            return Result<OwnedString, Err>::err(strerror(errno));
        }
        str.data[size] = 0;
        return Result<OwnedString, Err>::ok(str);
    }

    long len() {
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, 0, SEEK_SET);
        return size;
    };

    bool writeAll(const char *data) {
        fputs(data, file);
    }

    void close() {
        fclose(file);
    }
};
#endif //ALTLIB_FILE_H
