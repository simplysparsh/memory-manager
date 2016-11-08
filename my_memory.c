/*****************************************************************
 *
 * File             : my_memory.c
 * Description      :
 *
 * Author           : Sparsh Saxena
 * Last Modified    : November 8th, 2016
 *
******************************************************************/

// Include files
#include <stdio.h>
#include <stdlib.h>
#include "memalloc.h"

typedef struct Node Node_t; 
struct Node {
    void* start_addr;
    void* end_addr;
    int size;
    Node_t* link;
};

// Global Variables
int malloc_type = -1;
int mem_size = 0;
void* start_of_memory = NULL;
void* end_of_memory = NULL; 
int int_size = sizeof(int);

// Function Declarations
Node_t* create_new_node(void* start_addr, void* end_addr);
Node_t* get_allocatable_hole(int requested_size);
void add_to_allocated_mem(Node_t* node);
void add_to_holes_list(Node_t* node);
int delete_from_holes_list(Node_t* node);
void update_the_hole(Node_t* hole, int total_size);
Node_t* search_in_allocated_mem(void* address);
int delink_from_allocated_mem(Node_t* node);
Node_t* hole_with_same_end_addr(void* start_addr);
Node_t* hole_with_same_start_addr(void* end_addr);


//Lists to maintain memory usage
Node_t* holes = NULL;
Node_t* allocated_mem = NULL;

void setup( int _malloc_type, int _mem_size, void* _start_of_memory ) {

    malloc_type = _malloc_type;
    mem_size = _mem_size;
    start_of_memory = _start_of_memory; 
    end_of_memory = _start_of_memory + mem_size;

    holes = create_new_node(start_of_memory, end_of_memory);
}

void* my_malloc(int requested_size) {
    Node_t* allocatable_hole = NULL;
    Node_t* allocated_node = NULL;

    switch (malloc_type) {
        
        case FIRST_FIT:
            allocatable_hole = get_allocatable_hole(requested_size + int_size);

            if(allocatable_hole != NULL) {
                allocated_node = create_new_node(allocatable_hole->start_addr, 
                                 allocatable_hole->start_addr+requested_size+int_size);

                if((requested_size + int_size) == allocatable_hole -> size){
                    add_to_allocated_mem(allocated_node);
                    delete_from_holes_list(allocatable_hole);
                    //store the size in the first 4 bytes.
                    *(int*)(allocated_node->start_addr) = requested_size;
                    printf("Offset: %d\n", (int)(allocated_node->start_addr + int_size - start_of_memory)/1024);
                    return (allocated_node->start_addr + int_size);
                }
                else {
                    update_the_hole(allocatable_hole, requested_size+int_size);
                    add_to_allocated_mem(allocated_node);
                    //store the size in the first 4 bytes.
                    *(int*)(allocated_node->start_addr) = requested_size;
                    printf("Offset: %d\n", (int)(allocated_node->start_addr + int_size-start_of_memory)/1024);
                    return (allocated_node->start_addr + int_size);
                }
            }
            
            break;

        case BEST_FIT:
            //code 
            break;

        case WORST_FIT:
            // code
            break;

        case BUDDY_SYSTEM:
            // code
            break;
    }

    return (void*)-1;
}

void my_free(void* ptr) {
    Node_t* node_to_free = NULL;
    void* header = ptr - int_size;
    Node_t* mergeable_hole_1 = NULL;
    Node_t* mergeable_hole_2 = NULL;

    //Get the node in allocated list with start_addr = header
    node_to_free = search_in_allocated_mem(header);

    if(node_to_free == NULL) {
        printf("ERROR: Pointer not in allocated memory");
    }
    else {
        delink_from_allocated_mem(node_to_free);

        mergeable_hole_1 = hole_with_same_end_addr(node_to_free->start_addr);
        mergeable_hole_2 = hole_with_same_start_addr(node_to_free->end_addr);

        if(mergeable_hole_1 != NULL && mergeable_hole_2 != NULL) {
            mergeable_hole_1 -> end_addr = mergeable_hole_2 -> end_addr;
            delete_from_holes_list(mergeable_hole_2);
        }
        else if(mergeable_hole_1 != NULL) {
            mergeable_hole_1 -> end_addr = node_to_free -> end_addr;
            free(node_to_free);
        }
        else if(mergeable_hole_2 != NULL) {
            mergeable_hole_2 -> start_addr = node_to_free -> start_addr;
            free(node_to_free);
        }
        else {
            add_to_holes_list(node_to_free);
        }
    }
    return;
}

int num_free_bytes() {

    Node_t* curr = NULL;
    int sum = 0;
    curr = holes;

    while(curr != NULL) {
        sum = sum + curr->size;
        curr = curr -> link;
    }

    return sum;
}

int num_holes() {

    Node_t* curr = NULL;
    int count = 0;
    curr = holes;

    while(curr != NULL) {
        count = count + 1;
        curr = curr -> link;
    }
    return count;
    
}

/* Helper Functions */

Node_t* create_new_node(void* start_addr, void* end_addr) {
    Node_t* new_node = NULL;

    //Allocate and initialize
    new_node = malloc(sizeof(Node_t));

    new_node -> start_addr = start_addr;
    new_node -> end_addr = end_addr;
    new_node -> size = (end_addr - start_addr);
    new_node -> link = NULL;

    return new_node;
}

Node_t* get_allocatable_hole(int requested_size) {
    Node_t* curr = NULL;
    curr = holes;

    while(curr != NULL) {
        if(requested_size <= (curr->size)) {
            return curr;
        } 
        else {
            curr = curr -> link;
        }
    }
    return NULL;
}

void add_to_allocated_mem(Node_t* node) {

    Node_t* curr = NULL;
    Node_t* prev = NULL;
    curr = allocated_mem;

    if(curr != NULL) {

        if(node->start_addr < allocated_mem->start_addr) {
            node -> link = allocated_mem;
            allocated_mem = node;
        }
        else {
            while(curr != NULL && (curr->start_addr < node->start_addr)) {
                prev = curr;
                curr = curr -> link;
            }
            node -> link = curr;
            prev -> link = node;
        }
    }
    else {
        allocated_mem = node;
    }
    return;
}

void add_to_holes_list(Node_t* node) {
    Node_t* curr = NULL;
    Node_t* prev = NULL;
    curr = holes;

    if(curr != NULL) {

        if(node->start_addr < holes->start_addr) {
            node -> link = holes;
            holes = node;
        }
        else {
            while(curr->start_addr < node->start_addr) {
                prev = curr;
                curr = curr -> link;
            }
            node -> link = curr;
            prev -> link = node;
        }
    }
    return;
}

int delete_from_holes_list(Node_t* node) {
    Node_t* curr = NULL;
    Node_t* prev = NULL;

    curr = holes;

    while(curr != NULL) {

        if(curr == node) {
            if(curr == holes) {
                holes = curr -> link;
                free(curr);
                return 1;
            }
            else {
                prev -> link = curr -> link;
                free(curr);
                return 1;
            }
        }
        else {
            prev = curr;
            curr = curr -> link;
        }
    }
    return 0;
}

int delink_from_allocated_mem(Node_t* node) {
    Node_t* curr = NULL;
    Node_t* prev = NULL;

    curr = allocated_mem;

    while(curr != NULL) {

        if(curr == node) {
            if(curr == holes) {
                holes = curr -> link;
                curr -> link = NULL;
                return 1;
            }
            else {
                prev -> link = curr -> link;
                curr -> link = NULL;
                return 1;
            }
        }
        else {
            prev = curr;
            curr = curr -> link;
        }
    }
    return 0;
}

void update_the_hole(Node_t* hole, int total_size) {
    hole -> start_addr = hole -> start_addr + total_size;
    hole -> size = ((hole -> end_addr) - (hole -> start_addr));
    return;
}

Node_t* search_in_allocated_mem(void* address) {
    Node_t* curr = allocated_mem;

    while(curr != NULL) {
        
        if(curr->start_addr == address) {
            return curr;
        }
        else {
            curr = curr -> link;
        } 
    }
    return NULL;
}

Node_t* hole_with_same_end_addr(void* start_addr) {
    Node_t* curr = NULL;
    curr = holes;

    while(curr != NULL) {
        if(start_addr == curr -> end_addr) {
            return curr;
        } 
        else {
            curr = curr -> link;
        }
    }
    return NULL;
}

Node_t* hole_with_same_start_addr(void* end_addr) {
    Node_t* curr = NULL;
    curr = holes;

    while(curr != NULL) {
        if(end_addr == curr -> start_addr) {
            return curr;
        } 
        else {
            curr = curr -> link;
        }
    }
    return NULL;
}

