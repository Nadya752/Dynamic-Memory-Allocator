#include "virtual_alloc.h"
#include "virtual_sbrk.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

void init_allocator(void * heapstart, uint8_t initial_size, uint8_t min_size) {

    // If heapstart is NULL or minimum size is larger than initial size, 
    // there is nothing to be done.
    if (heapstart == (void*)(-1) || min_size > initial_size){
        return;
    }

    // Updating program break should a bigger size is needed to initialise
    // the virtual heap.
    int32_t offset = (int32_t)pow(BASE, initial_size);
    int32_t given_size = (uintptr_t) virtual_sbrk(0) - (uintptr_t)heapstart;
    int32_t needed_size = (offset + sizeof(Iblock)+ sizeof(Block)) - given_size;
    void* virtual_heap = virtual_sbrk(needed_size);

    // Return if there is not enough memory to initialise.
    if ((void*)(-1) == virtual_heap){
        return;
    }
    
    // Setting up the Info block that points to start,
    // and assign the info about min and initial size.
    Iblock* info = (Iblock*) heapstart;
    info->min = min_size;
    info->size = initial_size;

    // Setting up the first block.
    uintptr_t charoff = (uintptr_t) heapstart + sizeof(Iblock) + offset;
    void* heapoff = (void*) charoff;

    Block* start = (Block*) heapoff;
    info->start = start;
    start->next = NULL;
    start->prev = NULL;
    start->state = FREE;
    start->size = initial_size;
    start->memaddr = heapstart+ sizeof(Iblock);

}

void split_block(Block* current){

    void* new_block = virtual_sbrk(sizeof(Block));

    // Return if there is not enough memory.
    if ((void*)(-1) == new_block){
        return;
    }

    Block* new = (Block*) new_block;
    current->size = (current->size)-1;
    Block* last= current;

    // Travelling to last block.
    while (last->next != NULL){
        last = last->next;
    }

    // Connecting last block to new block.
    new->next = NULL;
    new->prev = last;
    last->next = new;
    last = new;

    //Shifting the blocks to the right.
    int initial_split = 1;
    while(last->prev != current && last->prev != NULL){

        last->size = last->prev->size;
        last->state = last->prev->state;
        last->memaddr = last->prev->memaddr;
        last = last->prev;
        initial_split = 0;

    }

    // If it is the first ever split from the first ever block, last is same as current. 
    // So, the new block is the buddy.
    Block* buddy = NULL;
    if (initial_split){
        buddy = new;

    }else{
        buddy = last;

    }

    // Setting up the new block as the buddy of split block.
    int offset = (int)pow(BASE, current->size);
    uintptr_t bud_addr = (uintptr_t) current->memaddr + offset;
    buddy->prev = current;
    buddy->size = current->size;
    buddy->state = FREE;
    buddy->memaddr = (void*)bud_addr;

}

int find_buddy (Block* block, void* heapstart){

    // Finding the memory address of buddy.

    /* USYD CODE CITATION ACKNOWLEDGEMENT
	 * I declare that the following lines of code have been copied from the
     * website titled: "Buddy Memory allocation" with only minor changes and it is not my own work. 
     * 
     * Original URL
     * https://www.kuniga.me/blog/2020/07/31/buddy-memory-allocation.html
     * Last access April, 2021
     */
    uintptr_t offset = block->memaddr - (heapstart + sizeof(Iblock));
    uintptr_t offset2 = (uintptr_t) offset ^(1<<(block->size));
    uintptr_t bud_addr = offset2 + (uintptr_t)(heapstart + sizeof(Iblock));
    /* end of copied code */

    void* buddy = (void*) bud_addr;
    int change_buddy = UNAVAILABLE;

    // Checking if buddy is next block or previous block or neither.
    if (block->next != NULL ){
        if (block->next->memaddr == buddy && block->next->size == block->size){
            change_buddy = NEXT;
        }
    }   

    if (block->prev != NULL){
        if (block->prev->memaddr == buddy && block->prev->size == block->size){
            change_buddy = PREV;
        }
    }

    return change_buddy;

}

void merge_block(Block* current, void* heapstart){

    // Return if there are no other blocks to merge.
    if (current->next == NULL && current->prev == NULL){
        return;
    }

    // Finding buddy block.
    Block* merged = current;
    Block* buddy_block = NULL;
    int change_buddy = find_buddy(current, heapstart);

    if(change_buddy == NEXT){
        buddy_block = current->next;
        
    }else if (change_buddy == PREV){
        buddy_block = current->prev;
        merged = buddy_block;
    }
    
    // Keep merging block if buddy is free and not split up.
    int count_merged = 0;
    int buddy_not_split = 1;
    if (buddy_block != NULL){
        while (buddy_block->state == FREE && merged->next != NULL && buddy_not_split){

            merged->size = (merged->size)+1;
            Block* nomad = merged->next;

            // Shifting the blocks to the left,
            // because one block is merged.
            while(nomad->next != NULL){

                nomad->size = nomad->next->size;
                nomad->state = nomad->next->state;
                nomad->memaddr = nomad->next->memaddr;
                nomad = nomad->next;

            }

            // Setting the new last block's next as null.
            // If new last block is the first block, then it's next block is null.
            if (nomad->prev != NULL){
                nomad->prev->next = NULL;

            }else{
                nomad->next = NULL;

            }

            // Check if newly merged block can again be merged with its buddy.
            change_buddy = find_buddy(merged, heapstart);
            if (change_buddy == NEXT ){
                buddy_block = merged->next;

            }else if (change_buddy == PREV){
                buddy_block = merged->prev;
                merged = buddy_block;

            }else{
                buddy_not_split = 0;
            }

            count_merged++;

        } 
    }

    virtual_sbrk(-(count_merged*sizeof(Block)));

}

int get_size(void* heapstart, uint32_t size){

    // Find the best fit size.
    uint8_t minimum = ((Iblock*) heapstart)-> min;
    uint8_t init_size = ((Iblock*) heapstart)-> size; 
    int find_size = (int) pow(BASE, init_size);
    int min = (int) pow(BASE, minimum);

    // Return if size is bigger than the maximum size,
    // or if size is 0.
    if(size > find_size || size == 0){
        return -1;
    }

    // Keep dividing until find_size is right fit for size.
    // or until it became smaller than minimum size.
    while ((find_size/BASE) >= size && (find_size/BASE) >= min){
        find_size = find_size/BASE;
    }

    find_size = (int)log2(find_size);
    return find_size;

}

int allocatable_size (void * heapstart, uint32_t size, void* ptr){

    int is_found = 0;
    int find_size = get_size(heapstart, size);

    // Invalid size.
    if (find_size == -1){
        return is_found;
    }

    Block* current = ((Iblock*) heapstart)-> start;

    // See if there is enough space if current one is merged with its buddy (if free).
    int change_buddy = find_buddy(current, heapstart);
    if (change_buddy == NEXT){
        uint8_t total_size = current->size + current->next->size;
        if (current->next->state == FREE && (total_size >= find_size)){
            is_found = 1;
        }
    }else if (change_buddy == PREV){
        uint8_t total_size = current->size +  current->prev-> size;
        if (current->prev->state == FREE && (total_size >= find_size)){
            is_found = 1;
        }
    }

    // If not, then the whole heap is searched if there is available space.
    while (!is_found){

        if (current->size >= find_size && (current->state == FREE || current->memaddr == ptr)){
            is_found = 1;
    
        }else{
            
            // No space left.
            if (NULL == current->next){
                return is_found;
            }

            current = current->next;
        }

    }

    return is_found;
}


void * virtual_malloc(void * heapstart, uint32_t size) {

    void* mem_address = NULL;
    int find_size = get_size(heapstart, size);

    // Invalid size.
    if(find_size == -1){
        return NULL;
    }

    Block* current = ((Iblock*) heapstart)-> start; 
    Block* skim = current;
    int found = 0;

    // Search if there is an exact free space.
    while (!found && skim->next !=NULL){

        if (skim->size == find_size && skim->state == FREE){
            mem_address = skim->memaddr;
            skim->state = ALLOC;
            found = 1;
        }

        skim = skim->next;

    }

    // Split if there is no exact space, start splitting the first adequately large block.
    while (!found){

        // Allocate if exact space is found.
        if (current->size == find_size && current->state == FREE){
            mem_address = current->memaddr;
            current->state = ALLOC;
            found = 1;

        // Split the first adequately large block.
        }else if (current->size > find_size && current->state == FREE){
            split_block(current);

        // Keep searching.
        }else{
            // No space left.
            if (NULL == current->next){
                return NULL;
            }

            current = current->next;
        }

    }

    return mem_address;

}


int virtual_free(void * heapstart, void * ptr) {

    // Search for block requested to be freed.
    // Return 1 when ptr is null (no such memory address).
    Block* current = ((Iblock*) heapstart)-> start;
    while (ptr != current->memaddr){

        // No such memory address.
        if (NULL == current->next){
            return 1;
        }

        current = current->next;

    }

    // Free block and merge if necessary.
    current->state = FREE;
    merge_block(current, heapstart);
    return 0;

}

void * virtual_realloc(void * heapstart, void * ptr, uint32_t size) {
    // If ptr is null, do a regular malloc, 
    // and if size is 0, do a regular free.
    if (ptr == NULL){
        return virtual_malloc(heapstart, size);

    }else if (size == 0){
        virtual_free(heapstart, ptr);
        return NULL;

    }

    // Allocatable_size is too see if there are any available spaces before any action is done (freeing and merging).
    // If virtual_malloc is called directly, then we have to free the block first.
    // But if we free the block first and and then the space turns out to be unavailable, 
    // it is unnecessarily complicated to revert the blocks back to its previous state. 
    if (allocatable_size(heapstart, size, ptr)){
        virtual_free(heapstart, ptr);

    }else{
        // Not enough memory.
        return NULL;
    }

    // Reallocate contents.
    void* dest = virtual_malloc(heapstart, size);
    memmove(dest, ptr, size);

    return dest;
}

void virtual_info(void * heapstart) {

    // Return if heapstart is null.
    if (heapstart == (void*)(-1)){
        printf("Heapstart is Null.\n");
        return;
    }

    Block* current = ((Iblock*) heapstart)-> start;

    // Keep printing blocks from start until last block.
    while (current != NULL){

        int block_size = (int)pow(BASE, current->size);

        switch (current->state){

            case (FREE):
                printf("free %d\n", block_size);
                break;

            case(ALLOC):
                printf("allocated %d\n",block_size);
                break;

        }

        current = current->next;

    }

}