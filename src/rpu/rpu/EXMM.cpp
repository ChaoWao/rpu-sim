#include "EXMM.h"
#include "../RPU.h"
#include "IDEX.h"
#include "../mm/Memory.hpp"
#include "TU.h"

EXMM::EXMM(RPU *r) :  timestamp(0), rpu(r), pc(unsigned(int(-1))) {}

bool EXMM::timed_stall() {
    if (rpu->idex->opcode.read() == RVInstr::TTIAT || rpu->idex->opcode.read() == RVInstr::TTOAT || rpu->idex->opcode.read() == RVInstr::DELAY) {
        if (rpu->idex->r1.read() > rpu->tu->get_time()) {
            return true;
        }
    }
    return false;
}

bool EXMM::get_pc_src_ctl() {
    bool tmp = [=] {
        const Word branch_op = rpu->idex->branch_op.read();
        const Word op1 = rpu->idex->r1.read();
        const Word op2 = rpu->idex->r2.read();
        switch(branch_op){
            case CompOp::NOP: return false;
            case CompOp::EQ: return op1 == op2;
            case CompOp::NE: return op1 != op2;
            case CompOp::LT: return int(op1) < int(op2);
            case CompOp::LTU: return op1 < op2;
            case CompOp::GE: return int(op1) >= int(op2);
            case CompOp::GEU: return op1 >= op2;
            default: return false;
        }
    }();
    if (rpu->idex->jump.read() == 1 || (rpu->idex->branch.read() == 1 && tmp)) {
        return true;
    }
    return false;
}

Word EXMM::get_alu_result() {
    if (rpu->idex->opcode.read() == RVInstr::GETTI) {
        return rpu->tu->get_time();
    } else if (rpu->idex->opcode.read() == RVInstr::GETTS) {
        return timestamp.read();
    } else if (rpu->idex->opcode.read() == RVInstr::TTIAT || rpu->idex->opcode.read() == RVInstr::TTOAT) {
        return rpu->idex->r2.read();
    }
    const Word alu_op = rpu->idex->alu_op.read();
    const Word op1 = rpu->idex->alu_op1_ctl.read() == AluSrc1::PC ? rpu->idex->pc.read() : rpu->idex->r1.read();
    const Word op2 = rpu->idex->alu_op2_ctl.read() == AluSrc2::IMM ? rpu->idex->imm.read() : rpu->idex->r2.read();
    return [=] {
        switch (alu_op) {
            case ALUOp::ADD:
                return op1 + op2;
            case ALUOp::SUB:
                return op1 - op2;
            case ALUOp::AND:
                return op1 & op2;

            case ALUOp::OR:
                return op1 | op2;

            case ALUOp::XOR:
                return op1 ^ op2;

            case ALUOp::SL:
                return op1 << op2;

            case ALUOp::SRA:
                return Word(int(op1) >> op2);

            case ALUOp::SRL:
                return op1 >> op2;

            case ALUOp::LUI:
                return op2;

            case ALUOp::LT:
                return Word(int(op1) < int(op2) ? 1 : 0);

            case ALUOp::LTU:
                return Word(op1 < op2 ? 1 : 0);

            case ALUOp::NOP:
                return (Word)0xDEADBEEF;

            default:
                throw std::runtime_error("Invalid ALU opcode");
        }
    }();
}

Word EXMM::get_pc_des() {
    return get_alu_result();
}

Word EXMM::get_timestamp() {
    return timestamp.read();
}

void EXMM::hook() {
    if (rpu->idex->opcode.read() == RVInstr::TTOAT) {
        Mask m = {true, true, true, true};
        rpu->dmem->write(rpu->idex->r2.read()+4-rpu->dm_start, rpu->idex->r1.read(), m);
    }
    npc.write(rpu->idex->npc.read());
    alu_result.write(get_alu_result());
    if (rpu->idex->opcode.read() == RVInstr::TTOAT) {
        r2.write(rpu->idex->r3.read());
    } else {
        r2.write(rpu->idex->r2.read());
    }
    rd.write(rpu->idex->rd.read());
    reg_write.write(rpu->idex->reg_write.read());
    wb_src_ctl.write(rpu->idex->wb_src_ctl.read());
    mem_write.write(rpu->idex->mem_write.read());
    mem_ctrl.write(rpu->idex->mem_ctrl.read());
    pc.write(rpu->idex->pc.read());
    if (rpu->idex->opcode.read() == RVInstr::TTIAT) {
        timestamp.write(rpu->dmem->read(rpu->idex->r2.read()+4-rpu->dm_start));
    } else {
        timestamp.write(timestamp.read());
    }
}

void EXMM::tick(bool reset, bool we) {
    npc.tick(reset, we);
    alu_result.tick(reset, we);
    r2.tick(reset, we);
    rd.tick(reset, we);
    reg_write.tick(reset, we);
    wb_src_ctl.tick(reset, we);
    mem_write.tick(reset, we);
    mem_ctrl.tick(reset, we);
    pc.tick(reset, we);
}
