#include "main_mem.h"
#include "abstract_mem.h"

#include <string.h>
#include <assert.h>

DECLARE_ABSTRACT_MEM_LOAD(MainMem) {
    //
}

DECLARE_ABSTRACT_MEM_STORE(MainMem) {
    //
}

void MainMem_ctor(MainMem *self) {
    assert((self != NULL) && "MainMem *self ptr should not be NULL!");

    // initlaize base class
    AbstractMem_ctor(&self->abstract_mem_super);
    struct AbstractMemVtbl const vtbl = {
        .load = &SIGNATURE_ABSTRACT_MEM_LOAD(MainMem),
        .store = &SIGNATURE_ABSTRACT_MEM_STORE(MainMem)};
    self->abstract_mem_super.vtbl = &vtbl;
    // initialize self->mem
    memset(&self->mem, 0, sizeof(byte_t) * MAIN_MEM_SIZE);
}
