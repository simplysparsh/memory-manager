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

// Global Variables
int malloc_type = -1;
int mem_size = 0;
void* start_of_memory = NULL;
void* end_of_memory = NULL; 

void setup( int malloc_type, int mem_size, void* start_of_memory ) {

    malloc_type = malloc_type;
    mem_size = mem_size;
    start_of_memory = start_of_memory; 
    end_of_memory = start_of_memory + mem_size;
}

void *my_malloc(int size) {

    switch (malloc_type) {

        case FIRST_FIT:
            //code
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
