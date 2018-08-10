//
// Created by lasagnaphil on 7/29/18.
//

#include "Vector2.h"

template<>
void Vector2i::deserialize(json::Value json) {
    x = json.getArrayItem(0).asInt().unwrap();
    y = json.getArrayItem(1).asInt().unwrap();
}

template<>
json::Value Vector2i::serialize() {
    json::Value json = json::Value::createArray();
    json.addArrayItem(json::Value::createNumber(x));
    json.addArrayItem(json::Value::createNumber(y));
    return json;
}