#ifndef __ISS_H__
#define __ISS_H__

/* 64-KiB main memory by default */
#define MEM_SIZE 0x10000
// 0x is hex, 0x10000 = 16^4 = 2^16 = 2^6 * 2^10 = 2^6 * 1KB = 64KB

#include <stdbool.h>
#include "riscv.h"

typedef struct {
    reg_t current_pc, new_pc; // program counter
    reg_t gpr[32];            // general purpose registers
    uint8_t mem[MEM_SIZE];    // main memory
    uint8_t reg_for_getchar;  // MMIO register
    bool halt;                // determine whether to stop
} cpu_state_t;

/* functions to manipulate processor itself */
extern bool processor_init(cpu_state_t **proc_ptr);
extern void processor_fini(cpu_state_t **proc_ptr);

#endif
