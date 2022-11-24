#include <stddef.h>
#include <stdint.h>
#define BASE (2)

typedef enum {

    FREE,
    ALLOC

} state;

typedef enum{

    NEXT,
    PREV,
    UNAVAILABLE

} buddy_location;

typedef struct Block{

    struct Block* next;
    struct Block* prev;
    uint8_t state;
    uint8_t size;
    void* memaddr;

} Block;

typedef struct{

    uint8_t min;
    uint32_t size;
    Block* start;

} Iblock;

void init_allocator(void * heapstart, uint8_t initial_size, uint8_t min_size);

void split_block(Block* current);

int find_buddy (Block* block, void* heapstart);

void merge_block(Block* current, void* heapstart);

int get_size(void* heapstart, uint32_t size);

int allocatable_size (void * heapstart, uint32_t size, void* ptr);

void * virtual_malloc(void * heapstart, uint32_t size);

int virtual_free(void * heapstart, void * ptr);

void * virtual_realloc(void * heapstart, void * ptr, uint32_t size);

void virtual_info(void * heapstart);
