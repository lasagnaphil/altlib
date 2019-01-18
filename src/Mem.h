//
// Created by lasagnaphil on 2019-01-17.
//

#ifndef ALTLIB_MEMORY_H
#define ALTLIB_MEMORY_H

// From: https://github.com/embeddedartistry/embedded-resources/blob/master/examples/c/malloc_aligned.c

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef align_up
#define align_up(num, align) \
	(((num) + ((align) - 1)) & ~((align) - 1))
#endif

//Convenience macro for memalign, the linux API
#define memalign(align, size) aligned_malloc(align, size)

//Number of bytes we're using for storing the aligned pointer offset
typedef uint16_t offset_t;
#define PTR_OFFSET_SZ sizeof(offset_t)

/**
* aligned_malloc takes in the requested alignment and size
*	We will call malloc with extra bytes for our header and the offset
*	required to guarantee the desired alignment.
*/

void * aligned_malloc(size_t align, size_t size);
void aligned_free(void * ptr);

#endif //ALTLIB_MEMORY_H
