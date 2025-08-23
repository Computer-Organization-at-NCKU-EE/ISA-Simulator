#ifndef __MEM_H__
#define __MEM_H__

#include "abstract_mem.h"
#include "arch.h"

typedef struct {
    int first;
    int second;
} int_pair_t;
typedef struct memory_map_unit {
    int_pair_t addr_bound;
    AbstractMem *device_ptr;
} memory_map_unit_t;
typedef struct {
    unsigned num_device;
    memory_map_unit_t *memory_map_arr;
} MemoryMap;

/* Public APIs */
// member functions
extern int MemoryMap_ctor(MemoryMap *self);
extern void MemoryMap_dtor(MemoryMap *self);
extern int MemoryMap_add_device(MemoryMap *self, memory_map_unit_t new_device);
// generic load/store APIs
extern void MemoryMap_generic_load(MemoryMap *self, addr_t base_addr,
                                   unsigned length, byte_t *buffer);
extern void MemoryMap_generic_store(MemoryMap *self, addr_t base_addr,
                                    unsigned length, const byte_t *ref_data);

#endif
