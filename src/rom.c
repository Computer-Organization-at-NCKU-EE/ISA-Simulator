#include "rom.h"

#include "abstract_mem.h"
#include "common.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

DECLARE_ABSTRACT_MEM_LOAD (ROM) {
    Assert (self != NULL, "self should not be NULL");
    Assert (base_addr + length <= ROM_SIZE, "");
    Assert (length == 4, "");

    ROM *self_ = container_of (self, ROM, super);
    for (int i = 0; i < length; i++) {
        buffer[i] = self_->boot_rom[base_addr + i];
    }
}

DECLARE_ABSTRACT_MEM_STORE (ROM) {
    Panic ("ROM should not be modified!");
}

void ROM_ctor (ROM *self) {
    assert (self != NULL);
    AbstractMem_ctor (&self->super);
    static struct AbstractMemVtbl const vtbl = {
        .load = &SIGNATURE_ABSTRACT_MEM_LOAD (ROM), .store = &SIGNATURE_ABSTRACT_MEM_STORE (ROM)
    };
    self->super.vtbl = &vtbl; // replace vtbl of base class

    // initialize boot rom code
    // onl one instruction in boot rom: jal x1, 0x80000000
    byte_t init_code[] = { 0xef, 0x00, 0x00, 0x00 };
    memcpy (&self->boot_rom, &init_code, sizeof (byte_t) * ROM_SIZE);
}
