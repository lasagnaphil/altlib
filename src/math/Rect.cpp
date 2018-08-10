//
// Created by lasagnaphil on 7/29/18.
//

#include "Rect.h"

template<>
void Rect2i::deserialize(json::Value json) {
    x1 = json.getArrayItem(0).asInt().unwrap();
    x2 = json.getArrayItem(1).asInt().unwrap();
}

template<>
json::Value Rect2i::serialize() {
    json::Value json = json::Value::createArray();
    json.addArrayItem(json::Value::createNumber(x1));
    json.addArrayItem(json::Value::createNumber(x2));
    return json;
}