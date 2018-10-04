//
// Created by lasagnaphil on 10/4/18.
//

#include <cmath>
#include "Vector4.h"

template <>
double Vector4<double>::normalize() const {
    return sqrt(x*x+y*y+z*z+w*w);
}

