//
// Created by lasagnaphil on 7/22/18.
//

#ifndef ALTLIB_FILE_H
#define ALTLIB_FILE_H

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include "Option.h"
#include "Str.h"
#include "Result.h"

struct File {
    FILE* file;

    static Result<File, Err> open(const char* filename, const char* permissions) {
        File file;
        file.file = fopen(filename, permissions);
        if (file.file == NULL) {
            auto errorStr = String::fmt(
                    "Error while loading file %s with permissions %s:\n    %s",
                    filename, permissions, strerror(errno)).unwrap();
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
            auto errorStr = String::fmt("Error while reading file:\n    %s", strerror(errno)).unwrap();
            return Result<String, Err>::err(errorStr);
        }
        data[size] = 0;
        return Result<String, Err>::ok(str);
    }

    Result<Vec<char>, Err> readBytes() {
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, 0, SEEK_SET);
        Vec<char> bytes = Vec<char>::emptyWithSize(size);
        if (size != fread(bytes.data, sizeof(char), size, file)) {
            bytes.free();
            auto errorStr = String::fmt("Error while reading file:\n    %s", strerror(errno)).unwrap();
            return Result<Vec<char>, Err>::err(errorStr);
        }
        return Result<Vec<char>, Err>::ok(bytes);
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
