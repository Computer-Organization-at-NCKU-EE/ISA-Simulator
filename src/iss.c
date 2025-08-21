#include "iss.h"

#include "arch.h"
#include "mem_map.h"

#define MEM_SIZE 0x10000 // 64 KiB

struct iss_state {
    bool halt; // determine whether to stop
    arch_state_t arch_state; // RISC-V architectural states
    byte_t main_mem[MEM_SIZE]; // main mamory
    memory_map_t mem_map; // memory map which contains all general memroy devices (with LOAD/STORE capability)
};