#include "../../virtual_alloc.h"
#include "../../virtual_sbrk.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    void* virtual_heap = virtual_sbrk(50);
    init_allocator(virtual_heap, 14, 7);
    virtual_malloc(virtual_heap, 4097);
    virtual_malloc(virtual_heap, 257);
    virtual_malloc(virtual_heap, 513);
    virtual_malloc(virtual_heap, 1025);
    virtual_info(virtual_heap);
    
    return 0;
}
