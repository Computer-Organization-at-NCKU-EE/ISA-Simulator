#ifndef __ISS_H__
#define __ISS_H__

#include "arch.h"

#include <stdbool.h>

// forward declaration
typedef struct iss_state iss_state_t;

// Public APIs to manipulate the ISS object
// for initializetion and finalization
extern void iss_init(iss_state_t **iss_ptr);
extern void iss_fini(iss_state_t **iss_ptr);
// for Reference-Model-Based Verification with RTL model
extern void iss_init_mem_map(iss_state_t* iss_ptr);
extern void iss_get_main_memory(addr_t base_addr, unsigned length);
extern bool iss_set_main_memory();
extern arch_state_t iss_get_arch_state();
extern void iss_set_arch_state();

#endif
