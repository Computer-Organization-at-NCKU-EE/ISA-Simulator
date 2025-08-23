#include "iss.h"

#include "arch.h"
#include "core.h"
#include "mem_map.h"
#include "tick.h"

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#define ROM_SIZE 0x20
#define MEM_SIZE 0x10000 // 64 KiB main memory by default

struct iss {
    Core core;
};

int ISS_ctor(ISS *self) {
    assert(self != NULL);
    Core_ctor(&self->core);

    // configure memory map
    // add main memory
    // add text buffer
    // add halt flag
    return 0;
}

void ISS_dtor(ISS *self) { Core_dtor(&self->core); }

void ISS_step(ISS *ISS_ptr, unsigned long n_step) {
    for (int i = 0; i < n_step; i++) {
        Tick_tick((Tick *)(&ISS_ptr->core));
    }
}

arch_state_t ISS_get_arch_state(const ISS *self) {
    //
}

void ISS_set_arch_state(ISS *self, const arch_state_t ref_arch_state) {
    //
}

void ISS_get_main_memory(const ISS *self, const addr_t base_addr,
                         const unsigned int length, byte_t *buffer) {
    //
}

void ISS_set_main_memory(ISS *self, const addr_t base_addr,
                         const unsigned int length, const byte_t *ref_data) {
    //
}
