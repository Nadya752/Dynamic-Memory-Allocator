#include "virtual_alloc.h"
#include "virtual_sbrk.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void * virtual_sbrk(int32_t increment) {
    return sbrk(increment);
}

