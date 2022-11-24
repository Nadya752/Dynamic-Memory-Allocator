#include "../../virtual_alloc.h"
#include "../../virtual_sbrk.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    void* virtual_heap = virtual_sbrk(10);
    init_allocator(virtual_heap, 20, 16);
    void* ptr = virtual_malloc(virtual_heap, 1000000);
    virtual_realloc(virtual_heap, ptr, 1000);
    virtual_info(virtual_heap);

    return 0;
}
