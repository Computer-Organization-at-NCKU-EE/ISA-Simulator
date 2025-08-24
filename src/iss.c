#include "iss.h"

#include "common.h"
#include "arch.h"
#include "core.h"
#include "mem_map.h"
#include "tick.h"
#include "main_mem.h"
#include "rom.h"
#include "halt.h"
#include "text_buffer.h"

#include <stddef.h>
#include <stdbool.h>

struct iss {
    // core part (RISC-V processor)
    Core core;

    // MMIO devices
    ROM rom_mmio;
    MainMem main_mem_mmio;
    TextBuffer text_buffer_mmio;
    Halt halt_mmio;
};

int ISS_ctor(ISS *self) {
    assert(self != NULL);

    // call constructors
    Core_ctor(&self->core);
    ROM_ctor(&self->rom_mmio);
    MainMem_ctor(&self->main_mem_mmio);
    TextBuffer_ctor(&self->text_buffer_mmio);

    // add ROM into core's mmap
    addr_t ROM_base = 0x0;
    mmap_unit_t ROM_mmap_unit = {
        .addr_bound = {.first = ROM_base, .second = ROM_base + ROM_SIZE},
        .device_ptr = (AbstractMem *)&self->rom_mmio};
    Core_add_device(&self->core, ROM_mmap_unit);

    // add main memory into core's mmap
    addr_t main_mem_base = 0x8000000;
    mmap_unit_t main_mem_mmap_unit = {
        .addr_bound = {.first = main_mem_base,
                       .second = main_mem_base + MAIN_MEM_SIZE},
        .device_ptr = (AbstractMem *)&self->main_mem_mmio};
    Core_add_device(&self->core, main_mem_mmap_unit);

    // add text buffer into core's mmap
    addr_t text_buffer_base = 0xfffffff8;
    mmap_unit_t text_buffer_mmap_unit = {
        .addr_bound = {.first = text_buffer_base,
                       .second = text_buffer_base + 0x4},
        .device_ptr = (AbstractMem *)&self->text_buffer_mmio};
    Core_add_device(&self->core, text_buffer_mmap_unit);

    // add halt flag into core's mmap
    addr_t halt_base = 0xfffffffc;
    mmap_unit_t halt_mmap_unit = {
        .addr_bound = {.first = halt_base, .second = halt_base + 0x4},
        .device_ptr = (AbstractMem *)&self->halt_mmio};
    Core_add_device(&self->core, halt_mmap_unit);

    return 0;
}

void ISS_dtor(ISS *self) {
    // core destructor
    Core_dtor(&self->core);

    /*
     * self->core is the only one data member whose destructor is must be called
     */
}

void ISS_step(ISS *self, unsigned long n_step) {
    for (unsigned i = 0; i < n_step; i++) {
        // check halt flag
        if (unlikely(self->halt_mmio.halt_flag == false)) {
            return;
        }
        // tick all tickable devices (includes core itself)
        Tick_tick(&self->core.super);
        Tick_tick(&self->text_buffer_mmio.tick_super);
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
