#ifndef __MEM_H__
#define __MEM_H__

#include <stdint.h>

#define MEM_SIZE 0x10000

// define integer pair type
typedef struct {
    int first;
    int second;
} int_pair_t;

typedef struct _general_mem {
    // memory map (its size will be cofigured in constructor)
    int_pair_t *memory_map;
    // a byte memory
    uint8_t mem[MEM_SIZE];
    // a putchar buffer
    uint8_t putchar_buffer;
} general_mem_t;

#endif
