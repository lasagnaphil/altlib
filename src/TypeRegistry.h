//
// Created by lasagnaphil on 7/10/18.
//

#ifndef THESYSTEM_TYPEREGISTRY_H
#define THESYSTEM_TYPEREGISTRY_H

#include <cstdint>

class TypeRegistry {
public:
    template <typename T> static uint16_t getID();
};


#endif //THESYSTEM_TYPEREGISTRY_H
