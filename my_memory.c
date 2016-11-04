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

// Function Declarations
Node_t* create_new_node(void* start_addr, void* end_addr);
Node_t* get_allocatable_hole(int requested_size);
void add_to_allocated_mem(Node_t* node);
int delete_from_holes_list(Node_t* node);
void update_the_hole(Node_t* hole, int requested_size);

//Lists to maintain memory usage
Node_t* holes = NULL;
Node_t* allocated_mem = NULL;

void setup( int malloc_type, int mem_size, void* start_of_memory ) {

    malloc_type = malloc_type;
    mem_size = mem_size;
    start_of_memory = start_of_memory; 
    end_of_memory = start_of_memory + mem_size;

    holes = create_new_node(start_of_memory, end_of_memory);
}

void* my_malloc(int requested_size) {
    Node_t* allocatable_hole = NULL;
    Node_t* allocated_node = NULL;

    switch (malloc_type) {
        
        case FIRST_FIT:
            allocatable_hole = get_allocatable_hole(requested_size);

            if(allocatable_hole != NULL) {
                allocated_node = create_new_node(allocatable_hole->start_addr, 
                                 allocatable_hole->start_addr+requested_size);

                if(requested_size == allocatable_hole -> size){
                    add_to_allocated_mem(allocated_node);
                    delete_from_holes_list(allocatable_hole);
                    return allocated_node;
                }
                else {
                    update_the_hole(allocatable_hole, requested_size);
                    add_to_allocated_mem(allocated_node);
                    return allocated_node;
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

void my_free(void *ptr) {
/**
 *  Fill your code here
 *
**/
}

int num_free_bytes() {
/**
 *  Fill your code here
 *
**/
    return 0;
}

int num_holes() {
/**
 *  Fill your code here
 *
**/
    return 0;
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
    curr = allocated_mem;

    if(curr != NULL) {
        while(curr->link != NULL) {
            curr = curr -> link;
        }
        curr -> link = node;       
    }
    else {
        allocated_mem = node;
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

void update_the_hole(Node_t* hole, int requested_size) {
    hole -> start_addr = hole -> start_addr + requested_size + 1;
    hole -> size = ((hole -> end_addr) - (hole -> start_addr));
    return;
}

