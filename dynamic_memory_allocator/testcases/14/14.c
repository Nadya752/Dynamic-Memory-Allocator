#include "../../virtual_alloc.h"
#include "../../virtual_sbrk.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    void* virtual_heap = virtual_sbrk(50);
    init_allocator(virtual_heap, 12, 6);
    void* ptr = virtual_malloc(virtual_heap, 64);
    void* ptr1 = virtual_malloc(virtual_heap, 64);
    virtual_free(virtual_heap, ptr);
    virtual_info(virtual_heap);
    virtual_free(virtual_heap, ptr1);
    virtual_info(virtual_heap);
    
    return 0;
}
