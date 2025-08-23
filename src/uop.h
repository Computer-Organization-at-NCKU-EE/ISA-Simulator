#ifndef __UOP_H__
#define __UOP_H__

#include "arch.h"

#include <stdint.h>
#include <stdbool.h>

typedef enum alu_op {
    Add,
    Sll,
    Slt,
    Sltu,
    Xor,
    Srl,
    Or,
    And,
    Sub,
    Sra
} alu_op_t;

typedef enum alu_op1_sel { reg_src1, current_pc, zero } alu_op1_sel_t;

typedef enum alu_op2_sel { reg_src2, imm_sign_ext } alu_op2_sel_t;

typedef enum branch_type {
    dump_directly,
    eq,
    neq,
    lt,
    ltu,
    ge,
    geu
} branch_type_t;

typedef enum rd_write_source_sel {
    pc_plus_4,
    alu_result
} rd_write_source_sel_t;

// micro-Op type
typedef struct uop {
    // about source register
    uint8_t reg_rs1_index, reg_rs2_index, rd_index;
    reg_t reg_rs1_val, reg_rs2_val;

    // abouut ALU operations
    alu_op1_sel_t alu_op1_sel;
    alu_op2_sel_t alu_op2_sel;
    alu_op_t alu_op;

    // about load/store
    bool is_mem, is_store;

    // about branch/jump instructions
    bool is_branch;
    branch_type_t branch_type;

    // about $rd write-back
    rd_write_source_sel_t rd_write_sel;
} uop_t;

#endif