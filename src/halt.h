#ifndef __HALT_H__
#define __HALT_H__

#include "abstract_mem.h"
#include "tick.h"

#include <stdbool.h>

typedef struct {
    // super classes
    AbstractMem super;
    // internal data
    bool halt_flag;
} Halt;

void Halt_ctor(Halt *self);

#endif
