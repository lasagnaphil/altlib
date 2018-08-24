//
// Created by lasagnaphil on 7/10/18.
//

#ifndef ALTLIB_TYPEREGISTRY_H
#define ALTLIB_TYPEREGISTRY_H

#include <stdint.h>

class TypeRegistry {
public:
    template <typename T> static uint16_t getID();
};


#endif //ALTLIB_TYPEREGISTRY_H
