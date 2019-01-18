//
// Created by lasagnaphil on 2019-01-17.
//

#include "Mem.h"

void * aligned_malloc(size_t align, size_t size)
{
    void * ptr = NULL;

    //We want it to be a power of two since align_up operates on powers of two
    assert((align & (align - 1)) == 0);

    if(align && size)
    {
        /*
         * We know we have to fit an offset value
         * We also allocate extra bytes to ensure we can meet the alignment
         */
        uint32_t hdr_size = PTR_OFFSET_SZ + (align - 1);
        void * p = malloc(size + hdr_size);

        if(p)
        {
            /*
             * Add the offset size to malloc's pointer (we will always store that)
             * Then align the resulting value to the arget alignment
             */
            ptr = (void *) align_up(((uintptr_t)p + PTR_OFFSET_SZ), align);

            //Calculate the offset and store it behind our aligned pointer
            *((offset_t *)ptr - 1) = (offset_t)((uintptr_t)ptr - (uintptr_t)p);

        } // else NULL, could not malloc
    } //else NULL, invalid arguments

    return ptr;
}

/**
* aligned_free works like free(), but we work backwards from the returned
* pointer to find the correct offset and pointer location to return to free()
* Note that it is VERY BAD to call free() on an aligned_malloc() pointer.
*/
void aligned_free(void * ptr)
{
    assert(ptr);

    /*
    * Walk backwards from the passed-in pointer to get the pointer offset
    * We convert to an offset_t pointer and rely on pointer math to get the data
    */
    offset_t offset = *((offset_t *)ptr - 1);

    /*
    * Once we have the offset, we can get our original pointer and call free
    */
    void * p = (void *)((uint8_t *)ptr - offset);
    free(p);
}
