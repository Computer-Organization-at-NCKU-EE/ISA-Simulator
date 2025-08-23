#include "halt.h"

#include "abstract_mem.h"
#include "common.h"

#include <stddef.h>
#include <assert.h>

DECLARE_ABSTRACT_MEM_LOAD(Halt) {
    // assertions
    assert(self != NULL);
    assert(buffer != NULL);
    assert(((base_addr == 0) && (length == 1)) && "Halt assertion fail!");

    // load into buffer
    Halt *self_ = container_of(self, Halt, super);
    buffer[0] = (byte_t)self_->halt_flag;
}

DECLARE_ABSTRACT_MEM_STORE(Halt) {
    // assertions
    assert(self != NULL);
    assert(ref_data != NULL);
    assert(((base_addr == 0) && (length == 1)) && "Halt assertion fail!");

    // load ref_data into Halt internal flag
    Halt *self_ = container_of(self, Halt, super);
    self_->halt_flag = (bool)(ref_data[0] & 0x1);
}

void Halt_ctor(Halt *self) {
    assert((self != NULL) && "Halt *self should not be null ptr");

    AbstractMem_ctor(&self->super);
    struct AbstractMemVtbl const vtbl = {
        .load = &SIGNATURE_ABSTRACT_MEM_LOAD(Halt),
        .store = &SIGNATURE_ABSTRACT_MEM_STORE(Halt)};
    self->super.vtbl = &vtbl;
}
