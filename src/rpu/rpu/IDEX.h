#ifndef RPU_SIMULATOR_DECODE_H
#define RPU_SIMULATOR_DECODE_H

#include "../include/Common.h"
#include "../include/Register.hpp"

class RPU;

class IDEX {
    friend class Hazard;
private:
    // decoder wire
    Word _rs1, _rs2, _rs3, _rd, _opcode;
    // immediate wire
    Word _imm;
    // control wire
    Word _reg_write, _wb_src_ctl;
    Word _mem_write, _mem_ctrl;
    Word _jump, _branch, _alu_op1_ctl, _alu_op2_ctl, _mem_read, _alu_op, _branch_op;
    // calculate wire
    void decoder();
    void immediate();
    void control();
    
public:
    RPU *rpu;
    // id/ex data
    // used by JAL(R), link pc+4
    Register npc;
    // used by AUIPC, JAL(R), BRANCH
    Register pc;
    Register r1, r2, r3, imm;
    Register rd;
    // information needed by forwarding and hazard unit
    Register rs1, rs2, rs3, opcode;
    // id/ex control
    Register reg_write, wb_src_ctl;
    Register mem_write, mem_ctrl;
    Register jump, branch, alu_op1_ctl, alu_op2_ctl, mem_read, alu_op, branch_op;
    // id/ex information for forwarding and hazard unit
    // this is used to track instructions in pipeline
    // pc above

    IDEX(RPU *r);
    void hook();
    void tick(bool reset, bool we);
    // used to calculate
    static Word bin_mask(unsigned int digits) {
        return (1 << digits) - 1;
    }

    static Word get_digits(unsigned int n, int hi, int lo) {
        return (n >> lo) & bin_mask(hi - lo + 1);
    }

    static Word expand_digit(unsigned int digit, int lo) {
        return digit ? (0xffffffff << lo) : 0;
    }

};

#endif
