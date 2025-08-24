#ifndef __UOP_H__
#define __UOP_H__

#include "arch.h"

#include <stdint.h>
#include <stdbool.h>

// define necessary enumerations for micro-Op
typedef enum { Add, Sll, Slt, Sltu, Xor, Srl, Or, And, Sub, Sra } alu_op_t;
typedef enum { reg_rs1, current_pc, zero } alu_op1_sel_t;
typedef enum { reg_rs2, imm_sign_ext } alu_op2_sel_t;
typedef enum { jump_anyway, eq, neq, lt, ltu, ge, geu } branch_type_t;
typedef enum { one, two, four } mem_length_t;
typedef enum { pc_plus_4, alu_result, mem_load, skip } rd_write_source_sel_t;

// micro-Op type
typedef struct uop {
    // information about the instruction itself
    reg_t pc;

    // about source register
    uint8_t reg_rs1_index, reg_rs2_index, reg_rd_index;
    reg_t reg_rs1_val, reg_rs2_val;

    // signed-extended immediate
    reg_t imm_signext;

    // abouut ALU operations
    alu_op1_sel_t alu_op1_sel;
    alu_op2_sel_t alu_op2_sel;
    alu_op_t alu_op;

    // about load/store
    bool is_mem, is_store, load_signext;
    mem_length_t mem_length;

    // about branch/jump instructions
    bool is_branch;
    branch_type_t branch_type;

    // about $rd write-back
    rd_write_source_sel_t rd_write_sel;
} uop_t;

#endif
