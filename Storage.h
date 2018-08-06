//
// Created by lasagnaphil on 2/21/18.
//

#ifndef GENGINE_STORAGE_H
#define GENGINE_STORAGE_H

#include <cassert>
#include <cstring>

#include "../TypeRegistry.h"
#include "String.h"
#include "Option.h"
#include "Symbol.h"
#include "File.h"
#include "Defer.h"
#include "json/Json.h"

template <typename T>
struct Ref {
    uint32_t index;
    uint16_t generation;
    uint16_t tid;

    static Ref fromInt64(int64_t data) {
        return *reinterpret_cast<Ref*>(&data);
    }
    int64_t ToInt64() {
        return *reinterpret_cast<int64_t*>(this);
    }

    static Ref fromStr(ImString str) {
        assert(str.len == 18);
        int64_t data = strtoll(str.data, NULL, 0);
        return Ref::fromInt64(data);
    }
    static Ref fromStr(const char* str) {
        assert(strlen(str) == 18);
        int64_t data = strtoll(str, NULL, 0);
        return Ref::fromInt64(data);
    }

    OwnedString toStr() {
        OwnedString str = OwnedString::create("", 18);
        snprintf(str.data, 18, "0x%llx", *reinterpret_cast<uint64_t*>(this));
        return str;
    }
};

template <typename T>
struct Storage {

    struct ItemNode {
        T item;
        uint32_t nextIndex;
        uint16_t generation;
        uint16_t free : 1;
    };

    ItemNode* nodes;

    uint32_t count;
    uint32_t capacity;

    uint32_t firstAvailable;

    uint16_t tid;

    static Storage create(uint32_t capacity) {
        Storage storage;
        storage.count = 0;
        storage.capacity = capacity;
        storage.firstAvailable = 0;
        storage.tid = TypeRegistry::getID<T>();
        storage.init();
        return storage;
    }

    void free() {
        delete[] nodes;
    }

    void init() {
        nodes = new ItemNode[capacity];
        for (uint32_t i = 0; i < capacity; ++i) {
            nodes[i].nextIndex = i + 1;
            nodes[i].generation = 0;
            nodes[i].free = 1;
        }
    }

    void expand(uint32_t newCapacity) {
        ItemNode* tempList = nodes;
        nodes = new ItemNode[newCapacity];
        std::memcpy(nodes, tempList, sizeof(ItemNode) * capacity);
        for (uint32_t i = capacity; i < newCapacity; ++i) {
            nodes[i].nextIndex = i + 1;
            nodes[i].generation = 0;
            nodes[i].free = 1;
        }

        capacity = newCapacity;

        delete[] tempList;
    }

    T& insert(T data, Ref<T> *ref_ptr = nullptr) {
        // if the item list is full
        if (firstAvailable == capacity) {
            expand(capacity * 2);
        }

        // delete node from free list
        uint32_t newIndex = firstAvailable;
        ItemNode& node = nodes[newIndex];
        firstAvailable = node.nextIndex;

        node.generation++;
        node.free = 0;

        count++;

        node.item = data;

        // also return the reference object of the resource
        if (ref_ptr) {
            ref_ptr->index = newIndex;
            ref_ptr->generation = node.generation;
            ref_ptr->tid = tid;
        }

        return node.item;
    }

    bool has(Ref<T> ref) const {
        ItemNode& node = nodes[ref.index];
        return !node.free && node.generation == ref.generation;
    }

    T& get(Ref<T> ref) const {
        ItemNode& node = nodes[ref.index];

        assert(!node.free);
        assert(node.generation == ref.generation);

        return node.item;
    }

    T* tryGet(Ref<T> ref) const {
        ItemNode& node = nodes[ref.index];

        if (!node.free && node.generation == ref.generation) {
            return &node.item;
        }
        else {
            return nullptr;
        }
    }

    void release(Ref<T> ref) {
        ItemNode& node = nodes[ref.index];

        assert(!node.free);
        assert(node.generation == ref.generation);

        node.free = 1;
        std::swap(node.nextIndex, firstAvailable);
        count--;
    }

    template <typename Fun>
    void iterate(Fun fun) {
        for (uint32_t i = 0; i < capacity; ++i) {
            ItemNode& node = nodes[i];
            if (!node.free) {
                fun(node.item);
            }
        }
    }

    static Storage<T> loadFromFile(const char* filename) {
        using namespace json;
        File file = File::open(filename, "rb+").unwrap();
        OwnedString str = file.readAll().unwrap();
        defer {str.free();};
        file.close();

        Value root = Value::parse(str.data).unwrap();

        Storage<T> storage;
        storage.deserialize(root);
        return storage;
    }

    void deserialize(json::Value root) {
        using namespace json;

        capacity = (uint32_t) root.get("capacity").asInt().unwrap();
        count = (uint32_t) root.get("count").asInt().unwrap();
        firstAvailable = (uint32_t) root.get("firstAvailable").asInt().unwrap();
        tid = (uint16_t) root.get("tid").asInt().unwrap();

        init();

        Value nodesRoot = root.get("nodes");
        for (int i = 0; i < capacity; ++i) {
            auto nodeRoot = nodesRoot.getArrayItem(i);

            nodes[i].nextIndex = nodeRoot.get("nextIndex").asInt().unwrap();
            nodes[i].generation = nodeRoot.get("generation").asInt().unwrap();
            nodes[i].free = nodeRoot.get("free").asInt().unwrap();
            if (!nodes[i].free) {
                nodes[i].item.deserialize(nodeRoot.get("item"));
            }

            /*
            const char* metaStr = nodeRoot.get("meta").asStr().unwrap();
            uint64_t meta = strtoull(metaStr, NULL, 0);
            nodes[i].nextIndex = (uint32_t)(meta & ((1ull << 32) - 1));
            nodes[i].generation = (uint16_t)((meta >> 32) & ((1ull << 16) - 1));
            nodes[i].free = (uint16_t)((meta >> 48) & ((1ull << 16) - 1));
            */

            auto item = nodeRoot.get("item");
            nodes[i].item.deserialize(item);
        }
    }

    json::Value serialize() {
        using namespace json;
        Value json = Value::createObject();
        json.addField("capacity", (int)capacity);
        json.addField("count", (int)count);
        json.addField("firstAvailable", (int)firstAvailable);
        json.addField("tid", tid);
        Value nodesRoot = Value::createArray();
        json.addField("nodes", nodesRoot);
        for (int i = 0; i < capacity; ++i) {
            Value node = Value::createObject();
            node.addField("nextIndex", (int)nodes[i].nextIndex);
            node.addField("generation", (int)nodes[i].generation);
            node.addField("free", (int)nodes[i].free);
            if (!nodes[i].free) {
                node.addField("item", nodes[i].item.serialize());
            }
            nodesRoot.addArrayItem(node);
        }
        return json;
    }

    void saveToFile(const char* filename) {
        File file = File::open(filename, "wb").unwrap();
        json::Value json = serialize();
        char* str = json.print();
        file.writeAll(str);
        file.close();
    }
};

#endif //GENGINE_RESOURCEMANAGER_H
