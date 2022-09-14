#pragma once

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>

#define PAGESIZE 4096
#define MAX_OBJ_SIZE 1024
#define NUM_OF_CACHES 10

void allocator_init(void);
void allocator_cleanup(void);


typedef struct header_t{                                                                                              
	int *free_list;
    struct header_t *previous, *next;                                                                                
    int obj_size;                                                                                     
    ssize_t available;                                                                                   
}header_t;                                                                                                     
                                                                                                              
void* malloc( size_t);
void* calloc( size_t count, size_t size);
void* realloc( void* ptr, size_t size);
void free( void* ptr);
void *Mmap(void* , size_t , int, int, int, off_t);
void Memcpy( void*, const void*, size_t);
