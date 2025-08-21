#ifndef __MEM_H__
#define __MEM_H__

#include <stdint.h>

#define MEM_SIZE 0x10000

// define integer pair type
typedef struct {
    int first;
    int second;
} int_pair_t;

typedef struct memory_map_unit {
    int_pair_t addr_bound;
    void *device_ptr;
} memory_map_unit_t;

typedef struct memory_map {
    unsigned num_device;
    memory_map_unit_t* memory_map_arr;
} memory_map_t;

#endif
