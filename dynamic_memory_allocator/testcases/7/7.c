#include "../../virtual_alloc.h"
#include "../../virtual_sbrk.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    void* virtual_heap = virtual_sbrk(50);
    init_allocator(virtual_heap, 12, 8);
    virtual_malloc(virtual_heap, 100);
    virtual_malloc(virtual_heap, 50);
    virtual_malloc(virtual_heap, 12);
    virtual_malloc(virtual_heap, 1);
    virtual_malloc(virtual_heap, 2);
    virtual_malloc(virtual_heap, 3);
    virtual_malloc(virtual_heap, 4);
    virtual_malloc(virtual_heap, 5);
    virtual_malloc(virtual_heap, 6);
    virtual_malloc(virtual_heap, 7);
    virtual_malloc(virtual_heap, 8);
    virtual_malloc(virtual_heap, 9);
    virtual_malloc(virtual_heap, 10);
    virtual_malloc(virtual_heap, 255);
    virtual_info(virtual_heap);


    return 0;
}
