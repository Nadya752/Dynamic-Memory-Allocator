#include "../../virtual_alloc.h"
#include "../../virtual_sbrk.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    void* virtual_heap =(void*)(-1);
    init_allocator(virtual_heap, 18, 11);
    virtual_info(virtual_heap);

    return 0;
}
