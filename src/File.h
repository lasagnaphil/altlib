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
            auto errorStr = String::fmt(
                    "Error while loading file %s with permissions %s:\n    %s",
                    filename, permissions, strerror(errno));
            return Result<File, Err>::err(errorStr);
        }
        return Result<File, Err>::ok(file);
    }

    Result<String, Err> readAll() {
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, 0, SEEK_SET);
        String str = String::create(size);
        char* data = str.data();
        if (size != fread(data, sizeof(char), size, file)) {
            str.free();
            auto errorStr = String::fmt("Error while reading file:\n    %s", strerror(errno));
            return Result<String, Err>::err(errorStr);
        }
        data[size] = 0;
        return Result<String, Err>::ok(str);
    }

    long len() {
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, 0, SEEK_SET);
        return size;
    };

    bool writeAll(const char *data) {
        int rc = fputs(data, file);
        return rc != EOF;
    }

    void close() {
        fclose(file);
    }
};
#endif //ALTLIB_FILE_H
