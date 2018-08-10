//
// Created by lasagnaphil on 7/28/18.
//

#ifndef THESYSTEM_JSON_H
#define THESYSTEM_JSON_H

#include "../log.h"
#include "../String.h"
#include "../Result.h"
#include "cJSON.h"

/*
 * C++ Wrapper for cJSON library
 */

static bool checkValid(cJSON *json) {
    if (json == NULL) {
        const char* errorPtr = cJSON_GetErrorPtr();
        log_error("JSON Load error (before %s)", errorPtr);
        return false;
    }
    return true;
}

namespace json {
    struct Value {
        cJSON* json;

        static Result<Value, Err> parse(const char* str) {
            cJSON* obj = cJSON_Parse(str);
            if (checkValid(obj)) {
                return Result<Value, Err>::ok(Value{obj});
            }
            else {
                return Result<Value, Err>::err(str);
            }
        }

        static Value createArray() {
            auto obj = cJSON_CreateArray();
            checkValid(obj);
            return Value {obj};
        };

        static Value createArrayRef(Value& child) {
            auto obj = cJSON_CreateArrayReference(child.json);
            checkValid(obj);
            return Value {obj};
        };

        static Value createObject() {
            auto obj = cJSON_CreateObject();
            checkValid(obj);
            return Value {obj};
        }

        static Value createObjectRef(Value& child) {
            auto obj = cJSON_CreateObjectReference(child.json);
            checkValid(obj);
            return Value {obj};
        }

        static Value createTrue() {
            auto obj = cJSON_CreateTrue();
            checkValid(obj);
            return Value {obj};
        }

        static Value createFalse() {
            auto obj = cJSON_CreateTrue();
            checkValid(obj);
            return Value {obj};
        }

        static Value createNull() {
            auto obj = cJSON_CreateNull();
            checkValid(obj);
            return Value {obj};
        };

        static Value createNumber(double num) {
            auto obj = cJSON_CreateNumber(num);
            checkValid(obj);
            return Value {obj};
        };

        static Value createStr(const char *string) {
            auto obj = cJSON_CreateString(string);
            checkValid(obj);
            return Value {obj};
        };

        static Value createIntArray(const int* numbers, int count) {
            auto obj = cJSON_CreateIntArray(numbers, count);
            checkValid(obj);
            return Value {obj};
        }

        static Value createFloatArray(const float* numbers, int count) {
            auto obj = cJSON_CreateFloatArray(numbers, count);
            checkValid(obj);
            return Value {obj};
        }

        static Value createDoubleArray(const double* numbers, int count) {
            auto obj = cJSON_CreateDoubleArray(numbers, count);
            checkValid(obj);
            return Value {obj};
        }

        static Value createStringArray(const char** strings, int count) {
            auto obj = cJSON_CreateStringArray(strings, count);
            checkValid(obj);
            return Value {obj};
        }

        Value get(const char *fieldStr) {
            cJSON* field = cJSON_GetObjectItemCaseSensitive(json, fieldStr);
            if (field == NULL) {
                const char* errorPtr = cJSON_GetErrorPtr();
                if (errorPtr) {
                    log_error("JSON error (field %s not found)", fieldStr);
                }
            }
        }

        Result<const char*, Err> asStr() {
            if (cJSON_IsString(json)) {
                return Result<const char*, Err>::ok(json->valuestring);
            }
            else {
                auto str = OwnedString::fmt("JSON error: %s", cJSON_GetErrorPtr());
                return Result<const char*, Err>::err(str.data);
            }
        }

        Result<int, Err> asInt() {
            if (cJSON_IsNumber(json)) {
                return Result<int, Err>::ok(json->valueint);
            }
            else {
                auto str = OwnedString::fmt("JSON error: %s", cJSON_GetErrorPtr());
                return Result<int, Err>::err(str.data);
            }
        }

        Result<double, Err> asDouble() {
            if (cJSON_IsNumber(json)) {
                return Result<double, Err>::ok(json->valuedouble);
            }
            else {
                auto str = OwnedString::fmt("JSON error: %s", cJSON_GetErrorPtr());
                return Result<double, Err>::err(str.data);
            }
        }

        bool isNull() {
            return cJSON_IsNull(json) == 0;
        }

        bool isNumber() {
            return cJSON_IsNumber(json) == 0;
        }

        bool isArray() {
            return cJSON_IsArray(json) == 0;
        }

        int getArraySize() {
            assert(cJSON_IsArray(json));
            return cJSON_GetArraySize(json);
        }

        Value getArrayItem(int index) {
            assert(cJSON_IsArray(json));
            return Value{cJSON_GetArrayItem(json, index)};
        }

        char* print() {
            return cJSON_Print(json);
        }

        char* printUnformatted() {
            return cJSON_PrintUnformatted(json);
        }

        void addField(const char* fieldStr, Value obj) {
            cJSON_AddItemToObject(json, fieldStr, obj.json);
        }

        void addField(const char* fieldStr, int i) {
            cJSON_AddNumberToObject(json, fieldStr, i);
        }

        void addField(const char* fieldStr, double d) {
            cJSON_AddNumberToObject(json, fieldStr, d);
        }

        void addField(const char* fieldStr, const char* str) {
            cJSON_AddStringToObject(json, fieldStr, str);
        }

        void addField(const char* fieldStr, bool b) {
            cJSON_AddBoolToObject(json, fieldStr, b);
        }

        void addArrayItem(Value item) {
            assert(cJSON_IsArray(json));
            cJSON_AddItemToArray(json, item.json);
        }

        void addArrayItemRef(Value item) {
            assert(cJSON_IsArray(json));
            cJSON_AddItemReferenceToArray(json, item.json);
        }

        void free() {
            cJSON_Delete(json);
        }
    };
};


#endif //THESYSTEM_JSON_H
