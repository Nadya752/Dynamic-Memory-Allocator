#include "../../virtual_alloc.h"
#include "../../virtual_sbrk.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    void* virtual_heap = virtual_sbrk(50);
    init_allocator(virtual_heap, 12, 7);
    virtual_malloc(virtual_heap, 2048);
    virtual_malloc(virtual_heap, 256);
    virtual_malloc(virtual_heap, 512);
    virtual_malloc(virtual_heap, 1024);
    virtual_info(virtual_heap);


    return 0;
}
