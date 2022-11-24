#include "../../virtual_alloc.h"
#include "../../virtual_sbrk.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    void* virtual_heap = virtual_sbrk(50);
    init_allocator(virtual_heap, 12, 7);
    void* ptr = virtual_malloc(virtual_heap, 256);
    void* ptr1 = virtual_malloc(virtual_heap, 256);
    void* ptr2 = virtual_malloc(virtual_heap, 256);
    void* ptr3 = virtual_malloc(virtual_heap, 512);
    virtual_info(virtual_heap);
    void* newptr = virtual_realloc(virtual_heap, ptr, 500);
    void* newptr1 =virtual_realloc(virtual_heap, ptr1, 128);
    virtual_info(virtual_heap);
    void* ptr4 = virtual_malloc(virtual_heap, 1000);
    void* ptr5 = virtual_malloc(virtual_heap, 500);
    void* newptr5 =virtual_realloc(virtual_heap, ptr5, 700);
    virtual_info(virtual_heap);
    virtual_free(virtual_heap, newptr1);
    virtual_free(virtual_heap, ptr3);
    virtual_free(virtual_heap, ptr2);
    virtual_free(virtual_heap, newptr);
    virtual_free(virtual_heap, ptr4);
    virtual_free(virtual_heap, newptr5);
    virtual_info(virtual_heap);

    return 0;
}

