#include "../../virtual_alloc.h"
#include "../../virtual_sbrk.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    void* virtual_heap = virtual_sbrk(100);
    init_allocator(virtual_heap, 11, 18);
    virtual_info(virtual_heap);

    return 0;
}
