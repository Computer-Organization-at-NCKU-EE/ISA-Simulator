#ifndef __ISS_H__
#define __ISS_H__

#include "arch.h"

// forward declaration
typedef struct iss ISS;

// for initializetion and finalization
extern int iss_init(ISS *self);
extern void iss_fini(ISS *self);

// for Reference-Model-Based Verification with RTL model
extern void iss_get_main_memory(const ISS *self, const addr_t base_addr,
                                const unsigned length, byte_t *const buffer);
extern void iss_set_main_memory(ISS *self, const addr_t base_addr,
                                const unsigned length,
                                const byte_t *const ref_data);
extern arch_state_t iss_get_arch_state(const ISS *self);
extern void iss_set_arch_state(ISS *self, const arch_state_t ref_arch_state);
extern void iss_step(ISS *self, unsigned long n_step);

#endif
