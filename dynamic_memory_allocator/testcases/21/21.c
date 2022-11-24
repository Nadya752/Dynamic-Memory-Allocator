#include "../../virtual_alloc.h"
#include "../../virtual_sbrk.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    void* virtual_heap = virtual_sbrk(50);
    init_allocator(virtual_heap, 12, 7);
    void* ptr = virtual_malloc(virtual_heap, 4000);
    void* ptr1 = virtual_realloc(virtual_heap, NULL, 512);
    virtual_info(virtual_heap);
    virtual_free(virtual_heap, ptr1);
    virtual_free(virtual_heap, ptr);
    virtual_info(virtual_heap);

    return 0;
}
