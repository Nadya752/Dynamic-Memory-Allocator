#include "../../virtual_alloc.h"
#include "../../virtual_sbrk.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    void* virtual_heap = virtual_sbrk(50);
    init_allocator(virtual_heap, 15, 9);
    void* ptr = virtual_malloc(virtual_heap, 512);
    void* ptr1 = virtual_malloc(virtual_heap, 1024);
    void* ptr2 = virtual_malloc(virtual_heap, 4096);
    void* ptr3 = virtual_malloc(virtual_heap, 16384);
    virtual_info(virtual_heap);
    virtual_free(virtual_heap, ptr3);
    virtual_free(virtual_heap, ptr2);
    virtual_free(virtual_heap, ptr1);
    virtual_free(virtual_heap, ptr);
    virtual_info(virtual_heap);

    return 0;
}
