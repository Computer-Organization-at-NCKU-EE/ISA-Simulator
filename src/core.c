#include "core.h"

#include "inst.h"
#include "uop.h"
#include "tick.h"
#include "arch.h"
#include "mem_map.h"
#include "common.h"

#include <assert.h>
#include <stdlib.h>

#define MEM_SIZE 0x10000 // 64 KiB

// ISS Fetch stage
static inst_t Core_fetch(Core *self) {
    //
}

// ISS decode stage
static uop_t Core_decode(Core *self, inst_t inst) {
    //
}

// ISS execute and commit stage (two-in-one)
static void Core_execute(Core *self, uop_t uop) {
    //
}

DECLARE_TICK_TICK(Core) {
    Core *self_ = container_of(self, Core, super);
    inst_t inst = Core_fetch(self_);
    uop_t uop = Core_decode(self_, inst);
    Core_execute(self_, uop);
}

void Core_ctor(Core *self) {
    assert(self != NULL);

    // add MMIO devices
    // TODO
    MemoryMap_ctor(&self->mem_map);

    // initialize base class
    Tick_ctor(&self->super);
    struct TickVtbl const vtbl = {.tick = SIGNATURE_TICK_TICK(Core)};
    self->super.vtbl = &vtbl;
}

void Core_dtor(Core *self) {
    assert(self != NULL);
    MemoryMap_dtor(&self->mem_map);
}

int Core_add_device(Core *self, memory_map_unit_t new_device) {
    return MemoryMap_add_device(&self->mem_map, new_device);
}
