#include "core.h"

#include "inst.h"
#include "uop.h"
#include "tick.h"
#include "arch.h"
#include "mem_map.h"
#include "common.h"

#include <stddef.h>
#include <assert.h>

// ISS Fetch stage
static inst_t Core_fetch(Core *self) {
    // fetch instruction according to self->arch_state.current_pc
    byte_t inst_in_bytes[4] = {};
    MemoryMap_generic_load(&self->mem_map, self->arch_state.current_pc, 4,
                           inst_in_bytes);
    // transformation
    inst_t ret = {};
    ret.raw |= (reg_t)inst_in_bytes[0];
    ret.raw |= (reg_t)inst_in_bytes[1] << 8;
    ret.raw |= (reg_t)inst_in_bytes[2] << 16;
    ret.raw |= (reg_t)inst_in_bytes[3] << 24;
    return ret;
}

// ISS decode stage
static uop_t Core_decode(Core *self, inst_t inst) {
    uop_t ret = {};
    ret.pc = self->arch_state.current_pc;

    // helper local variables
    reg_t opcode = inst.R_TYPE.opcode;
    reg_t rs1 = inst.R_TYPE.rs1;
    reg_t rs2 = inst.R_TYPE.rs2;
    reg_t rd = inst.R_TYPE.rd;
    reg_t func3 = inst.R_TYPE.func3;
    reg_t func7 = inst.R_TYPE.func7;

    // decode common part
    switch (opcode) {
    case OP: {
        ret.reg_rs1_index = rs1;
        ret.reg_rs2_index = rs2;
        ret.reg_rd_index = rd;
        ret.reg_rs1_val = self->arch_state.gpr[rs1];
        ret.reg_rs2_val = self->arch_state.gpr[rs2];
        ret.alu_op1_sel = reg_rs1;
        ret.alu_op2_sel = reg_rs2;
        ret.rd_write_sel = alu_result;
        break;
    }
    case OP_IMM: {
        ret.reg_rs1_index = rs1;
        ret.reg_rd_index = rd;
        ret.reg_rs1_val = self->arch_state.gpr[rs1];
        ret.imm_signext = inst.I_TYPE.imm_11_0;
        ret.alu_op1_sel = reg_rs1;
        ret.alu_op2_sel = imm_sign_ext;
        ret.rd_write_sel = alu_result;
        break;
    }
    case LOAD: {
        ret.reg_rs1_index = rs1;
        ret.reg_rd_index = rd;
        ret.reg_rs1_val = self->arch_state.gpr[rs1];
        ret.imm_signext = inst.I_TYPE.imm_11_0;
        ret.alu_op1_sel = reg_rs1;
        ret.alu_op2_sel = imm_sign_ext;
        ret.rd_write_sel = alu_result;
        ret.is_mem = true;
        break;
    }
    case STORE: {
        ret.reg_rs1_index = rs1;
        ret.reg_rs2_index = rs2;
        ret.reg_rs1_val = self->arch_state.gpr[rs1];
        ret.reg_rs2_val = self->arch_state.gpr[rs2];
        ret.imm_signext = (inst.S_TYPE.imm_11_5 << 5) + inst.S_TYPE.imm_4_0;
        ret.alu_op1_sel = reg_rs1;
        ret.alu_op2_sel = imm_sign_ext;
        ret.is_mem = true;
        ret.is_store = true;
        ret.rd_write_sel = skip;
        break;
    }
    case BRANCH: {

        ret.reg_rs1_index = rs1;
        ret.reg_rs2_index = rs2;
        ret.reg_rs1_val = self->arch_state.gpr[rs1];
        ret.reg_rs2_val = self->arch_state.gpr[rs2];
        ret.imm_signext =
            (inst.B_TYPE.imm_12 << 12) + (inst.B_TYPE.imm_11 << 11) +
            (inst.B_TYPE.imm_10_5 << 5) + (inst.B_TYPE.imm_4_1 << 1);
        ret.alu_op1_sel = reg_rs1;
        ret.alu_op2_sel = imm_sign_ext;
        ret.is_branch = true;
        ret.rd_write_sel = skip;
        break;
    }
    case JAL: {
        ret.reg_rd_index = rd;
        ret.imm_signext =
            (inst.J_TYPE.imm_20 << 20) + (inst.J_TYPE.imm_19_12 << 12) +
            (inst.J_TYPE.imm_11 << 11) + (inst.J_TYPE.imm_10_1 << 1);
        ret.alu_op1_sel = current_pc;
        ret.alu_op2_sel = imm_sign_ext;
        ret.alu_op = Add;
        ret.is_branch = true;
        ret.branch_type = jump_anyway;
        ret.rd_write_sel = pc_plus_4;
        break;
    }
    case JALR: {
        ret.reg_rd_index = rd;
        ret.imm_signext = inst.I_TYPE.imm_11_0;
        ret.alu_op1_sel = reg_rs1;
        ret.alu_op2_sel = imm_sign_ext;
        ret.alu_op = Add;
        ret.is_branch = true;
        ret.branch_type = jump_anyway;
        ret.rd_write_sel = pc_plus_4;
        break;
    }
    case AUIPC: {
        ret.reg_rd_index = rd;
        ret.imm_signext = inst.U_TYPE.imm_31_12 << 12;
        ret.alu_op1_sel = current_pc;
        ret.alu_op2_sel = imm_sign_ext;
        ret.alu_op = Add;
        ret.rd_write_sel = alu_result;
        break;
    }
    case LUI: {
        ret.reg_rd_index = rd;
        ret.imm_signext = inst.U_TYPE.imm_31_12 << 12;
        ret.alu_op1_sel = zero;
        ret.alu_op2_sel = imm_sign_ext;
        ret.alu_op = Add;
        ret.rd_write_sel = alu_result;
        break;
    }
    }

    // decode alu_op
    switch (inst.R_TYPE.opcode) {
    case OP:
    case OP_IMM: {
        switch (func3) {
        case ADD_SUB_FUNC3: {
            if (opcode == OP) {
                if (func7) {
                    ret.alu_op = Sub;
                } else {
                    ret.alu_op = Add;
                }
            } else {
                ret.alu_op = Add;
            }
            break;
        }
        case SLL_FUNC3: {
            ret.alu_op = Sll;
            break;
        }
        case SLT_FUNC3: {
            ret.alu_op = Slt;
            break;
        }
        case SLTU_FUNC3: {
            ret.alu_op = Sltu;
            break;
        }
        case XOR_FUNC3: {
            ret.alu_op = Xor;
            break;
        }
        case SRL_SRA_FUNC3: {
            if (func7) {
                ret.alu_op = Sra;
            } else {
                ret.alu_op = Srl;
            }
            break;
        }
        case OR_FUNC3: {
            ret.alu_op = Or;
            break;
        }
        case AND_FUNC3: {
            ret.alu_op = And;
        }
        }
        break;
    }
    default: {
    }
    }

    // decode mem_length and load_signext
    if (opcode == LOAD || opcode == STORE) {
        switch (func3) {
        case LBU_FUNC3:
        case SB_FUNC3: {
            ret.mem_length = 1;
            break;
        }
        case SH_FUNC3:
        case LHU_FUNC3: {
            ret.mem_length = 2;
            break;
        }
        default: {
            ret.mem_length = 4;
        }
        }
    }
    if (opcode == LOAD && func3 != LBU_FUNC3 && func3 != LHU_FUNC3) {
        ret.load_signext = true;
    }

    // decode types of conditional-branch
    if (opcode == BRANCH) {
        switch (func3) {
        case BEQ_FUNC3: {
            ret.branch_type = eq;
            break;
        }
        case BNE_FUNC3: {
            ret.branch_type = neq;
            break;
        }
        case BLT_FUNC3: {
            ret.branch_type = lt;
            break;
        }
        case BGE_FUNC3: {
            ret.branch_type = ge;
            break;
        }
        case BLTU_FUNC3: {
            ret.branch_type = ltu;
            break;
        }
        case BGEU_FUNC3: {
            ret.branch_type = geu;
        }
        }
    }

    return ret;
}

// ISS execute and commit stage (two-in-one)
static void Core_execute(Core *self, uop_t uop) {
    // set self->new_pc to a default value by PC+4
    // it might be overridden when there is a branch instruction
    self->new_pc = self->arch_state.current_pc + 4;

    // preset op1 and op2 for ALU calculation afterward
    reg_t op1 = 0;
    switch (uop.alu_op1_sel) {
    case reg_rs1: {
        op1 = uop.reg_rs1_val;
        break;
    }
    case current_pc: {
        op1 = uop.pc;
        break;
    }
    default: {
    }
    }

    reg_t op2 = 0;
    switch (uop.alu_op2_sel) {
    case reg_rs2: {
        op2 = uop.reg_rs2_val;
        break;
    }
    case imm_sign_ext: {
        op2 = uop.imm_signext;
        break;
    }
    default: {
    }
    }

    // perform ALU calculation according to uop.alu_op
    reg_t alu_cal_result = 0;
    switch (uop.alu_op) {
    case Add: {
        alu_cal_result = op1 + op2;
        break;
    }
    case Sll: {
        alu_cal_result = op1 << (op2 & 0x1f);
        break;
    }
    case Slt: {
        alu_cal_result = ((int32_t)op1 < (int32_t)op2) ? 1 : 0;
        break;
    }
    case Sltu: {
        alu_cal_result = (op1 < op2) ? 1 : 0;
        break;
    }
    case Xor: {
        alu_cal_result = op1 ^ op2;
        break;
    }
    case Srl: {
        alu_cal_result = op1 >> (op2 & 0x1f);
        break;
    }
    case Or: {
        alu_cal_result = op1 | op2;
        break;
    }
    case And: {
        alu_cal_result = op1 & op2;
        break;
    }
    case Sub: {
        alu_cal_result = op1 - op2;
        break;
    }
    case Sra: {
        alu_cal_result = (int32_t)op1 << (op2 & 0x1f);
        break;
    }
    }

    // handle memory instruction
    reg_t mem_load_buffer = 0;
    if (uop.is_mem) {
        // check load/store length (in byte)
        unsigned len = 0;
        switch (uop.mem_length) {
        case one: {
            len = 1;
            break;
        }
        case two: {
            len = 2;
            break;
        }
        case four: {
            len = 4;
        }
        }

        // perform load/store
        if (uop.is_store) {
            byte_t _ref_data[4] = {};
            for (int i = 0; i < len; i++) {
                _ref_data[i] = (uop.reg_rs2_val >> (8 * i)) & 0xff;
            }
            MemoryMap_generic_store(&self->mem_map, alu_cal_result, len,
                                    _ref_data);
        } else {
            byte_t _buffer[4] = {};
            MemoryMap_generic_load(&self->mem_map, alu_cal_result, len,
                                   _buffer);
            if (uop.load_signext && len != 4) {
                switch (len) {
                case 1: {
                    mem_load_buffer |= _buffer[0];
                    if (_buffer[1] >> 7) {
                        mem_load_buffer |= 0xffffff << 8;
                    }
                    break;
                }
                case 2: {
                    mem_load_buffer |= _buffer[0];
                    mem_load_buffer |= _buffer[1];
                    if (_buffer[2] >> 7) {
                        mem_load_buffer |= 0xffff << 16;
                    }
                    break;
                }
                default: {
                }
                }
            } else {
                mem_load_buffer |= _buffer[0];
                mem_load_buffer |= _buffer[1] << 8;
                mem_load_buffer |= _buffer[2] << 16;
                mem_load_buffer |= _buffer[3] << 24;
            }
        }
    }

    // handle control-flow instruction (jump or conditional-branch)
    // set new_pc in need
    if (uop.is_branch) {
        switch (uop.branch_type) {
        case jump_anyway: {
            self->new_pc = alu_cal_result;
            break;
        }
        case eq: {
            if (uop.reg_rs1_val == uop.reg_rs2_val) {
                self->new_pc = alu_cal_result;
            }
            break;
        }
        case neq: {
            if (uop.reg_rs1_val != uop.reg_rs2_val) {
                self->new_pc = alu_cal_result;
            }
            break;
        }
        case lt: {
            if ((int32_t)uop.reg_rs1_val < (int32_t)uop.reg_rs2_val) {
                self->new_pc = alu_cal_result;
            }
            break;
        }
        case ge: {
            if ((int32_t)uop.reg_rs1_val >= (int32_t)uop.reg_rs2_val) {
                self->new_pc = alu_cal_result;
            }
            break;
        }
        case ltu: {
            if (uop.reg_rs1_val < uop.reg_rs2_val) {
                self->new_pc = alu_cal_result;
            }
            break;
        }
        case geu: {
            if (uop.reg_rs1_val >= uop.reg_rs2_val) {
                self->new_pc = alu_cal_result;
            }
        }
        }
    }

    // perform write-back in need
    switch (uop.rd_write_sel) {
    case pc_plus_4: {
        if (uop.reg_rd_index != 0) {
            self->arch_state.gpr[uop.reg_rd_index] = uop.pc + 4;
        }
        break;
    }
    case alu_result: {
        if (uop.reg_rd_index != 0) {
            self->arch_state.gpr[uop.reg_rd_index] = alu_cal_result;
        }
        break;
    }
    case mem_load: {
        if (uop.reg_rd_index != 0) {
            self->arch_state.gpr[uop.reg_rd_index] = mem_load_buffer;
        }
        break;
    }
    case skip: {
    }
    }
}

static void Core_update_pc(Core *self) {
    self->arch_state.current_pc = self->new_pc;
}

DECLARE_TICK_TICK(Core) {
    Core *self_ = container_of(self, Core, super);
    inst_t inst = Core_fetch(self_);
    uop_t uop = Core_decode(self_, inst);
    Core_execute(self_, uop);
    Core_update_pc(self_);
}

void Core_ctor(Core *self) {
    assert(self != NULL);

    // initialize memory map object
    // prepare for being added new MMIO devices
    MemoryMap_ctor(&self->mem_map);

    // initialize base class (Tick)
    Tick_ctor(&self->super);
    static struct TickVtbl const vtbl = {.tick = SIGNATURE_TICK_TICK(Core)};
    self->super.vtbl = &vtbl;
}

void Core_dtor(Core *self) {
    assert(self != NULL);
    MemoryMap_dtor(&self->mem_map);
}

int Core_add_device(Core *self, mmap_unit_t new_device) {
    return MemoryMap_add_device(&self->mem_map, new_device);
}
