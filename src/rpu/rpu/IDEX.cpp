#include "IDEX.h"
#include "../RPU.h"
#include "IFID.h"
#include "RegisterFile.hpp"

void IDEX::decoder() {
    Word instr = rpu->ifid->instr.read();
    _opcode = [=] {
        const Word l7 = get_digits(instr, 6, 0);

        // clang-format off
        switch(l7) {
        case 0b0110111: return RVInstr::LUI;
        case 0b0010111: return RVInstr::AUIPC;
        case 0b1101111: return RVInstr::JAL;
        case 0b1100111: return RVInstr::JALR;

        case 0b0010011: {
            // I-Type
            const Word funct3 = get_digits(instr, 14, 12);
            switch(funct3) {
                case 0b000: return RVInstr::ADDI;
                case 0b010: return RVInstr::SLTI;
                case 0b011: return RVInstr::SLTIU;
                case 0b100: return RVInstr::XORI;
                case 0b110: return RVInstr::ORI;
                case 0b111: return RVInstr::ANDI;
                case 0b001: return RVInstr::SLLI;
                case 0b101: {
                    switch (instr >> 25) {
                    case 0b0: return RVInstr::SRLI;
                    case 0b0100000: return RVInstr::SRAI;
                    }
                }
                default: break;
            }
            break;
        }

        case 0b0110011: {
            // R-Type
            const Word funct3 = get_digits(instr, 14, 12);
            switch(funct3) {
                case 0b000: {
                    switch (instr >> 25) {
                        case 0b0000000: return RVInstr::ADD;
                        case 0b0100000: return RVInstr::SUB;
                    }
                }
                case 0b001: return RVInstr::SLL;
                case 0b010: return RVInstr::SLT;
                case 0b011: return RVInstr::SLTU;
                case 0b100: return RVInstr::XOR;
                case 0b101: {
                    switch (instr >> 25) {
                        case 0b0000000: return RVInstr::SRL;
                        case 0b0100000: return RVInstr::SRA;
                    }
                }
                case 0b110: return RVInstr::OR;
                case 0b111: return RVInstr::AND;
                default: break;
            }
            break;
        }

        case 0b0000011: {
            // Load instruction
            const Word funct3 = get_digits(instr, 14, 12);
            switch(funct3) {
                case 0b000: return RVInstr::LB;
                case 0b001: return RVInstr::LH;
                case 0b010: return RVInstr::LW;
                case 0b100: return RVInstr::LBU;
                case 0b101: return RVInstr::LHU;
                default: break;
            }
            break;
        }

        case 0b0100011: {
            // Store instructions
            const Word funct3 = get_digits(instr, 14, 12);
            switch(funct3) {
                case 0b000: return RVInstr::SB;
                case 0b001: return RVInstr::SH;
                case 0b010: return RVInstr::SW;
                default: break;
            }
            break;
        }

        case 0b1100011: {
            // Branch instruction
            const Word funct3 = get_digits(instr, 14, 12);
            switch(funct3) {
                case 0b000: return RVInstr::BEQ;
                case 0b001: return RVInstr::BNE;
                case 0b100: return RVInstr::BLT;
                case 0b101: return RVInstr::BGE;
                case 0b110: return RVInstr::BLTU;
                case 0b111: return RVInstr::BGEU;
                default: break;
            }
            break;
        }

        case 0b0001011: {
            // TT instruction
            const Word funct3 = get_digits(instr, 14, 12);
            switch (funct3) {
            case 0b000: {
                // Standard clock instruction
                const Word funct7 = get_digits(instr, 31, 25);
                switch (funct7) {
                case 0b0000000: return RVInstr::SETTG;
                case 0b0000001: return RVInstr::SETTI;
                case 0b0000010: return RVInstr::GETTI;
                case 0b0000011: return RVInstr::GETTS;
                default: break;
                }
                break;
            }
            case 0b001: {
                // Timed operation
                const Word funct7 = get_digits(instr, 31, 25);
                switch(funct7) {
                case 0b0000000: return RVInstr::TTIAT;
                case 0b0000001: return RVInstr::TTOAT;
                case 0b0000010: return RVInstr::DELAY;
                default: break;
                }
                break;
            }
            case 0b010: {
                // CMT operation
                const Word funct7 = get_digits(instr, 31, 25);
                switch (funct7) {
                case 0b0000000: return RVInstr::TKEND;
                case 0b0000001: return RVInstr::ADDTK;
                default: break;
                }
                break;
            }
            default: break;
            }
            break;
        }


        default:
            break;
        }

        // Fallthrough - unknown instruction.
        return RVInstr::NOP;
    }();

    _rs1 = (instr >> 15) & 0b11111;
    _rs2 = (instr >> 20) & 0b11111;
    _rd = (instr >> 7) & 0b11111;
    _rs3 = _opcode == RVInstr::TTOAT ? _rd : 0;
}

void IDEX::immediate() {
    Word instr = rpu->ifid->instr.read();
    _imm = [=] {
        switch(_opcode) {
            case RVInstr::LUI:
            case RVInstr::AUIPC:
                return instr & 0xfffff000;
            case RVInstr::JAL: {
                return (get_digits(instr, 30, 21) << 1) |
                        (get_digits(instr, 20, 20) << 11) |
                        (get_digits(instr, 19, 12) << 12) |
                        expand_digit(get_digits(instr, 31, 31), 20);
            }
            case RVInstr::JALR: {
                return get_digits(instr, 30, 20) | expand_digit(get_digits(instr, 31, 31), 11);
            }
            case RVInstr::BEQ:
            case RVInstr::BNE:
            case RVInstr::BLT:
            case RVInstr::BGE:
            case RVInstr::BLTU:
            case RVInstr::BGEU: {
                return (get_digits(instr, 11, 8) << 1) |
                        (get_digits(instr, 30, 25) << 5) |
                        (get_digits(instr, 7, 7) << 11) |
                        expand_digit(get_digits(instr, 31, 31), 12);
            }
            case RVInstr::LB:
            case RVInstr::LH:
            case RVInstr::LW:
            case RVInstr::LBU:
            case RVInstr::LHU:
            case RVInstr::ADDI:
            case RVInstr::SLTI:
            case RVInstr::SLTIU:
            case RVInstr::XORI:
            case RVInstr::ORI:
            case RVInstr::ANDI:
            case RVInstr::SLLI:
            case RVInstr::SRLI:
            case RVInstr::SRAI:
                return get_digits(instr, 30, 20) | expand_digit(get_digits(instr, 31, 31), 11);
            case RVInstr::SB:
            case RVInstr::SH:
            case RVInstr::SW: {
                return get_digits(instr, 11, 7) |
                        (get_digits(instr, 30, 25) << 5) |
                        expand_digit(get_digits(instr, 31, 31), 11);
            }
            default:
                return (Word)0xDEADBEEF;
        }
    }();
}

void IDEX::control() {
    _reg_write = [=] {
        switch(_opcode) {
            case RVInstr::LUI:
            case RVInstr::AUIPC:

            // Arithmetic-immediate instructions
            case RVInstr::ADDI: case RVInstr::SLTI: case RVInstr::SLTIU: case RVInstr::XORI:
            case RVInstr::ORI: case RVInstr::ANDI: case RVInstr::SLLI: case RVInstr::SRLI:
            case RVInstr::SRAI:

            // Arithmetic instructions
            case RVInstr::ADD: case RVInstr::SUB: case RVInstr::SLL: case RVInstr::SLT:
            case RVInstr::SLTU: case RVInstr::XOR: case RVInstr::SRL: case RVInstr::SRA:
            case RVInstr::OR: case RVInstr::AND:

            // Load instructions
            case RVInstr::LB: case RVInstr::LH: case RVInstr::LW: case RVInstr::LBU: case RVInstr::LHU:

            // Jump instructions
            case RVInstr::JALR:
            case RVInstr::JAL:

            // TT instructions
            case RVInstr::GETTI:
            case RVInstr::GETTS:
            case RVInstr::TTIAT:
                return 1;
            default: return 0;
        }
    }();
    
    _wb_src_ctl = [=] {
        switch(_opcode){
            // Load instructions
            case RVInstr::LB: case RVInstr::LH: case RVInstr::LW: case RVInstr::LBU: case RVInstr::LHU:
            // TTIAT
            case RVInstr::TTIAT:
                return RegWrSrc::MEMREAD;

            // Jump instructions
            case RVInstr::JALR:
            case RVInstr::JAL:
                return RegWrSrc::PC4;

            default:
                return RegWrSrc::ALURES;
        }
    }();

    _mem_write = [=] {
        switch(_opcode) {
            case RVInstr::SB: case RVInstr::SH: case RVInstr::SW: case RVInstr::TTOAT:
                return 1;
            default: return 0;
        }
    }();

    _mem_ctrl = [=] {
        switch(_opcode){
            case RVInstr::SB: return MemOp::SB;
            case RVInstr::SH: return MemOp::SH;
            case RVInstr::SW: case RVInstr::TTOAT: return MemOp::SW;
            case RVInstr::LB: return MemOp::LB;
            case RVInstr::LH: return MemOp::LH;
            case RVInstr::LW: case RVInstr::TTIAT: return MemOp::LW;
            case RVInstr::LBU: return MemOp::LBU;
            case RVInstr::LHU: return MemOp::LHU;
            default:
                return MemOp::NOP;
        }
    }();

    _jump = [=] {
        switch(_opcode){
            case RVInstr::JAL: case RVInstr::JALR:
                return 1;
            default:
                return 0;
        }
    }();

    _branch = [=] {
        switch(_opcode){
            case RVInstr::BEQ: case RVInstr::BNE: case RVInstr::BLT:
            case RVInstr::BGE: case RVInstr::BLTU: case RVInstr::BGEU:
                return 1;
            default:
                return 0;
        }
    }();

    _alu_op1_ctl = [=] {
        switch(_opcode) {
            case RVInstr::AUIPC: case RVInstr::JAL:
            case RVInstr::BEQ: case RVInstr::BNE: case RVInstr::BLT:
            case RVInstr::BGE: case RVInstr::BLTU: case RVInstr::BGEU:
                return AluSrc1::PC;
            default:
                return AluSrc1::REG1;
        }
    }();

    _alu_op2_ctl = [=] {
        switch(_opcode) {
        case RVInstr::LUI:
        case RVInstr::AUIPC:
            return AluSrc2::IMM;

        // Arithmetic-immediate instructions
        case RVInstr::ADDI: case RVInstr::SLTI: case RVInstr::SLTIU: case RVInstr::XORI:
        case RVInstr::ORI: case RVInstr::ANDI: case RVInstr::SLLI: case RVInstr::SRLI:
        case RVInstr::SRAI:
            return AluSrc2::IMM;

        // Arithmetic instructions
        case RVInstr::ADD: case RVInstr::SUB: case RVInstr::SLL: case RVInstr::SLT:
        case RVInstr::SLTU: case RVInstr::XOR: case RVInstr::SRL: case RVInstr::SRA:
        case RVInstr::OR: case RVInstr::AND:
            return AluSrc2::REG2;

        // Load/Store instructions
        case RVInstr::LB: case RVInstr::LH: case RVInstr::LW: case RVInstr::LBU: case RVInstr::LHU:
        case RVInstr::SB: case RVInstr::SH: case RVInstr::SW:
            return AluSrc2::IMM;

        // Branch instructions
        case RVInstr::BEQ: case RVInstr::BNE: case RVInstr::BLT:
        case RVInstr::BGE: case RVInstr::BLTU: case RVInstr::BGEU:
            return AluSrc2::IMM;

        // Jump instructions
        case RVInstr::JALR:
        case RVInstr::JAL:
            return AluSrc2::IMM;

        default:
            return AluSrc2::REG2;
        }
    }();
    
    _mem_read = [=] {
        switch(_opcode) {
            case RVInstr::LB: case RVInstr::LH: case RVInstr::LW: case RVInstr::LBU: case RVInstr::LHU: case RVInstr::TTIAT:
                return 1;
            default: return 0;
        }
    }();

    _alu_op = [=] {
        switch(_opcode) {
            case RVInstr::LB: case RVInstr::LH: case RVInstr::LW: case RVInstr::LBU: case RVInstr::LHU:
            case RVInstr::SB: case RVInstr::SH: case RVInstr::SW:
                return ALUOp::ADD;
            case RVInstr::LUI:
                return ALUOp::LUI;
            case RVInstr::JAL: case RVInstr::JALR: case RVInstr::AUIPC:
            case RVInstr::ADD: case RVInstr::ADDI:
            case RVInstr::BEQ: case RVInstr::BNE: case RVInstr::BLT:
            case RVInstr::BGE: case RVInstr::BLTU: case RVInstr::BGEU:
                return ALUOp::ADD;
            case RVInstr::SUB:
                return ALUOp::SUB;
            case RVInstr::SLT: case RVInstr::SLTI:
                return ALUOp::LT;
            case RVInstr::SLTU: case RVInstr::SLTIU:
                return ALUOp::LTU;
            case RVInstr::XOR: case RVInstr::XORI:
                return ALUOp::XOR;
            case RVInstr::OR: case RVInstr::ORI:
                return ALUOp::OR;
            case RVInstr::AND: case RVInstr::ANDI:
                return ALUOp::AND;
            case RVInstr::SLL: case RVInstr::SLLI:
                return ALUOp::SL;
            case RVInstr::SRL: case RVInstr::SRLI:
                return ALUOp::SRL;
            case RVInstr::SRA: case RVInstr::SRAI:
                return ALUOp::SRA;
            default: return ALUOp::NOP;
        }
    }();

    _branch_op = [=] {
        switch(_opcode){
            case RVInstr::BEQ: return CompOp::EQ;
            case RVInstr::BNE: return CompOp::NE;
            case RVInstr::BLT: return CompOp::LT;
            case RVInstr::BGE: return CompOp::GE;
            case RVInstr::BLTU: return CompOp::LTU;
            case RVInstr::BGEU: return CompOp::GEU;
            default: return CompOp::NOP;
        }
    }();

}

IDEX::IDEX(RPU *r) : rpu(r), pc(unsigned(int(-1))) {}

void IDEX::hook() {
    decoder();
    immediate();
    control();
    npc.write(rpu->ifid->npc.read());
    pc.write(rpu->ifid->pc.read());
    r1.write(rpu->rf->read(_rs1));
    r2.write(rpu->rf->read(_rs2));
    r3.write(rpu->rf->read(_rs3));
    imm.write(_imm);
    rd.write(_rd);
    rs1.write(_rs1);
    rs2.write(_rs2);
    rs3.write(_rs3);
    opcode.write(_opcode);
    reg_write.write(_reg_write);
    wb_src_ctl.write(_wb_src_ctl);
    mem_write.write(_mem_write);
    mem_ctrl.write(_mem_ctrl);
    jump.write(_jump);
    branch.write(_branch);
    alu_op1_ctl.write(_alu_op1_ctl);
    alu_op2_ctl.write(_alu_op2_ctl);
    mem_read.write(_mem_read);
    alu_op.write(_alu_op);
    branch_op.write(_branch_op);
}

void IDEX::tick(bool reset, bool we) {
    npc.tick(reset, we);
    pc.tick(reset, we);
    r1.tick(reset, we);
    r2.tick(reset, we);
    r3.tick(reset, we);
    imm.tick(reset, we);
    rd.tick(reset, we);
    rs1.tick(reset, we);
    rs2.tick(reset, we);
    rs3.tick(reset, we);
    opcode.tick(reset, we);
    reg_write.tick(reset, we);
    wb_src_ctl.tick(reset, we);
    mem_write.tick(reset, we);
    mem_ctrl.tick(reset, we);
    jump.tick(reset, we);
    branch.tick(reset, we);
    alu_op1_ctl.tick(reset, we);
    alu_op2_ctl.tick(reset, we);
    mem_read.tick(reset, we);
    alu_op.tick(reset, we);
    branch_op.tick(reset, we);
}
