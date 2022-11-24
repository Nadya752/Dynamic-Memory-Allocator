#include "../../virtual_alloc.h"
#include "../../virtual_sbrk.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    void* virtual_heap = virtual_sbrk(10);
    init_allocator(virtual_heap, 20, 16);
    virtual_malloc(virtual_heap, 500000);
    void* ptr1 = virtual_malloc(virtual_heap, 500000);
    virtual_realloc(virtual_heap, ptr1, 1000000);
    virtual_info(virtual_heap);

    return 0;
}