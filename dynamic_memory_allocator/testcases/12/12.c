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
    virtual_free(virtual_heap, ptr1);
    virtual_free(virtual_heap, ptr3);
    virtual_free(virtual_heap, ptr2);
    virtual_free(virtual_heap, ptr);
    virtual_info(virtual_heap);
    
    return 0;
}
