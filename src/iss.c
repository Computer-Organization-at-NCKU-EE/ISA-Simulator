#include "iss.h"

#include "common.h"
#include "arch.h"
#include "core.h"
#include "load_elf.h"
#include "mem_map.h"
#include "tick.h"
#include "main_mem.h"
#include "rom.h"
#include "halt.h"
#include "text_buffer.h"

#include <stddef.h>
#include <stdbool.h>

int ISS_ctor(ISS *self, const char *elf_file_name) {
    assert(self != NULL);

    // call constructors
    Core_ctor(&self->core);
    ROM_ctor(&self->rom_mmio);
    MainMem_ctor(&self->main_mem_mmio);
    TextBuffer_ctor(&self->text_buffer_mmio);
    Halt_ctor(&self->halt_mmio);

    // add ROM into core's mmap
    mmap_unit_t ROM_mmap_unit = {
        .addr_bound = {.first = ROM_MMAP_BASE,
                       .second = ROM_MMAP_BASE + ROM_SIZE},
        .device_ptr = (AbstractMem *)&self->rom_mmio};
    Core_add_device(&self->core, ROM_mmap_unit);

    // add main memory into core's mmap
    mmap_unit_t main_mem_mmap_unit = {
        .addr_bound = {.first = MAIN_MEM_MMAP_BASE,
                       .second = MAIN_MEM_MMAP_BASE + MAIN_MEM_SIZE},
        .device_ptr = (AbstractMem *)&self->main_mem_mmio};
    Core_add_device(&self->core, main_mem_mmap_unit);

    // add text buffer into core's mmap
    mmap_unit_t text_buffer_mmap_unit = {
        .addr_bound = {.first = TEXT_BUFFER_MMAP_BASE,
                       .second = TEXT_BUFFER_MMAP_BASE + TEXT_BUFFER_SIZE},
        .device_ptr = (AbstractMem *)&self->text_buffer_mmio};
    Core_add_device(&self->core, text_buffer_mmap_unit);

    // add halt flag into core's mmap
    mmap_unit_t halt_mmap_unit = {
        .addr_bound = {.first = HALT_MMAP_BASE,
                       .second = HALT_MMAP_BASE + HALT_SIZE},
        .device_ptr = (AbstractMem *)&self->halt_mmio};
    Core_add_device(&self->core, halt_mmap_unit);

    // load ELF into main memory, and initialize PC
    load_elf(elf_file_name, self->main_mem_mmio.mem, MAIN_MEM_SIZE,
             &self->core.arch_state.current_pc);

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
        if (unlikely(self->halt_mmio.halt_flag == true)) {
            printf("reg $gp: %x", self->core.arch_state.gpr[3]);
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
