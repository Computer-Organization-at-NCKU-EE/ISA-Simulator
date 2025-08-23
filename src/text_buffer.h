#ifndef __TEXT_BUFFER_H__
#define __TEXT_BUFFER_H__

#include "arch.h"
#include "abstract_mem.h"
#include "tick.h"

#include <stdbool.h>

typedef struct {
    // derived base class
    AbstractMem abstract_mem_super;
    Tick tick_super;
    // buffer for "one" character (one byte)
    bool valid;
    byte_t buffer;
} TextBuffer;

void TextBuffer_init(TextBuffer *self);

#endif
