#ifndef __ROM_H__
#define __ROM_H__

#include "abstract_mem.h"
#include "arch.h"

// define ROM byte size
#define ROM_SIZE 0x4

typedef struct {
    // parent class
    AbstractMem abstract_mem_base;

    // the rom itself
    byte_t boot_rom[ROM_SIZE];
} ROM;

extern void ROM_ctor(ROM *self);

#endif
