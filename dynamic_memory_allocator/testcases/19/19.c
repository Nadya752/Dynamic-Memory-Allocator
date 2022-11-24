#include "../../virtual_alloc.h"
#include "../../virtual_sbrk.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    void* virtual_heap = virtual_sbrk(50);
    init_allocator(virtual_heap, 15, 9);
    virtual_malloc(virtual_heap, 512);
    void* ptr = virtual_malloc(virtual_heap, 512);
    virtual_malloc(virtual_heap, 512);
    virtual_malloc(virtual_heap, 512);
    virtual_free(virtual_heap, ptr);
    virtual_free(virtual_heap, ptr);
    virtual_info(virtual_heap);

    return 0;
}
