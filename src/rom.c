#include "rom.h"
#include "abstract_mem.h"

#include <stdio.h>
#include <assert.h>

DECLARE_ABSTRACT_MEM_LOAD(ROM) {
    //
}

DECLARE_ABSTRACT_MEM_STORE(ROM) {
    //
}

void ROM_init(ROM *self) {
    assert(self != NULL);
    AbstractMem_ctor(&self->abstract_mem_base);
    static struct AbstractMemVtbl const vtbl = {
        .load = &SIGNATURE_ABSTRACT_MEM_LOAD(ROM),
        .store = &SIGNATURE_ABSTRACT_MEM_STORE(ROM)};
    self->abstract_mem_base.vtbl = &vtbl; // replace vtbl of base class

    // initialize boot rom code
    // byte_t init_code[] = {0x97, 0x2, 0x00, 0x00};
}
