#include "../../virtual_alloc.h"
#include "../../virtual_sbrk.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    void* virtual_heap = virtual_sbrk(50);
    init_allocator(virtual_heap, 15, 9);
    void* ptr = virtual_malloc(virtual_heap, 16000);
    virtual_free(virtual_heap, ptr);
    virtual_realloc(virtual_heap, ptr, 500);
    virtual_info(virtual_heap);

    return 0;
}
