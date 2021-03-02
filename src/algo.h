#ifndef ALGO_H
#define ALGO_H

#include "rpu/include/Common.h"
#include "rpu/rpu/IDEX.h"
#include <sstream>
#include <iomanip>
#include <QString>

class Algo {
public:
    //
    static QString unsigned2qstring(unsigned i) {
        std::stringstream stream;
        stream << "0x"
            << std::setfill ('0') << std::setw(8)
            << std::hex << i;
        return QString::fromStdString(stream.str());
    }
    //
    static QString int2qstring(int i) {
        std::stringstream stream;
        stream << i;
        return QString::fromStdString(stream.str());
    }
    //
    static QString instr2assemble(unsigned instr) {
        QString stream;
        unsigned _rs1 = (instr >> 15) & 0b11111;
        unsigned _rs2 = (instr >> 20) & 0b11111;
        unsigned _rd = (instr >> 7) & 0b11111;
        unsigned _opcode = [=, &stream] {
            const Word l7 = IDEX::get_digits(instr, 6, 0);

            // clang-format off
            switch(l7) {
            case 0b0110111:
                stream = "LUI ";
                return RVInstr::LUI;
            case 0b0010111:
                stream = "AUIPC ";
                return RVInstr::AUIPC;
            case 0b1101111:
                stream = "JAL ";
                return RVInstr::JAL;
            case 0b1100111:
                stream = "JALR ";
                return RVInstr::JALR;
            case 0b0010011: {
                // I-Type
                const Word funct3 = IDEX::get_digits(instr, 14, 12);
                switch(funct3) {
                case 0b000:
                    stream = "ADDI ";
                    return RVInstr::ADDI;
                case 0b010:
                    stream = "SLTI ";
                    return RVInstr::SLTI;
                case 0b011:
                    stream = "SLTIU ";
                    return RVInstr::SLTIU;
                case 0b100:
                    stream = "XORI ";
                    return RVInstr::XORI;
                case 0b110:
                    stream = "ORI ";
                    return RVInstr::ORI;
                case 0b111:
                    stream = "ANDI ";
                    return RVInstr::ANDI;
                case 0b001:
                    stream = "SLLI ";
                    return RVInstr::SLLI;
                case 0b101:
                    switch (instr >> 25) {
                    case 0b0:
                        stream = "SRLI ";
                        return RVInstr::SRLI;
                    case 0b0100000:
                        stream = "SRAI ";
                        return RVInstr::SRAI;
                    default: break;
                    }
                default: break;
                }
                break;
            }
            case 0b0110011: {
                // R-Type
                const Word funct3 = IDEX::get_digits(instr, 14, 12);
                switch(funct3) {
                case 0b000: {
                    switch (instr >> 25) {
                    case 0b0000000:
                        stream = "ADD ";
                        return RVInstr::ADD;
                    case 0b0100000:
                        stream = "SUB ";
                        return RVInstr::SUB;
                    default:
                        break;
                    }
                }
                case 0b001:
                    stream = "SLL ";
                    return RVInstr::SLL;
                case 0b010:
                    stream = "SLT ";
                    return RVInstr::SLT;
                case 0b011:
                    stream = "SLTU ";
                    return RVInstr::SLTU;
                case 0b100:
                    stream = "XOR ";
                    return RVInstr::XOR;
                case 0b101: {
                    switch (instr >> 25) {
                    case 0b0000000:
                        stream = "SRL ";
                        return RVInstr::SRL;
                    case 0b0100000:
                        stream = "SRA ";
                        return RVInstr::SRA;
                    }
                }
                case 0b110:
                    stream = "OR ";
                    return RVInstr::OR;
                case 0b111:
                    stream = "AND ";
                    return RVInstr::AND;
                default:
                    break;
                }
                break;
            }
            case 0b0000011: {
                // Load instruction
                const Word funct3 = IDEX::get_digits(instr, 14, 12);
                switch(funct3) {
                case 0b000:
                    stream = "LH ";
                    return RVInstr::LB;
                case 0b001:
                    stream = "LW ";
                    return RVInstr::LH;
                case 0b010:
                    stream = "LW ";
                    return RVInstr::LW;
                case 0b100:
                    stream = "LBU ";
                    return RVInstr::LBU;
                case 0b101:
                    stream = "LHU ";
                    return RVInstr::LHU;
                default:
                    break;
                }
                break;
            }
            case 0b0100011: {
                // Store instructions
                const Word funct3 = IDEX::get_digits(instr, 14, 12);
                switch(funct3) {
                case 0b000:
                    stream = "SB ";
                    return RVInstr::SB;
                case 0b001:
                    stream = "SH ";
                    return RVInstr::SH;
                case 0b010:
                    stream = "SW ";
                    return RVInstr::SW;
                default:
                    break;
                }
                break;
            }
            case 0b1100011: {
                // Branch instruction
                const Word funct3 = IDEX::get_digits(instr, 14, 12);
                switch(funct3) {
                case 0b000:
                    stream = "BEQ ";
                    return RVInstr::BEQ;
                case 0b001:
                    stream = "BNE ";
                    return RVInstr::BNE;
                case 0b100:
                    stream = "BLT ";
                    return RVInstr::BLT;
                case 0b101:
                    stream = "BGE ";
                    return RVInstr::BGE;
                case 0b110:
                    stream = "BLTU ";
                    return RVInstr::BLTU;
                case 0b111:
                    stream = "BGEU ";
                    return RVInstr::BGEU;
                default:
                    break;
                }
                break;
            }
            case 0b0001011: {
                // TT instructions
                const Word funct3 = IDEX::get_digits(instr, 14, 12);
                switch (funct3) {
                case 0b000: {
                    // TM Instructions
                    const Word funct7 = IDEX::get_digits(instr, 31, 25);
                    switch (funct7) {
                    case 0b0000000:
                        stream = "SETTG x" +  QString::number(_rs1);
                        return RVInstr::SETTG;
                    case 0b0000001:
                        stream = "SETTI x" +  QString::number(_rs1);
                        return RVInstr::SETTI;
                    case 0b0000010:
                        stream = "GETTI x" +  QString::number(_rd);
                        return RVInstr::GETTI;
                    case 0b0000011:
                        stream = "GETTS x" +  QString::number(_rs1);
                        return RVInstr::GETTS;
                    default:
                        break;
                    }
                    break;
                }
                case 0b001: {
                    const Word funct7 = IDEX::get_digits(instr, 31, 25);
                    switch (funct7) {
                    case 0b0000000:
                        stream = "TTIAT x" + QString::number(_rd) + ", x" + QString::number(_rs2) + ", x" + QString::number(_rs1);
                        return RVInstr::TTIAT;
                    case 0b0000001:
                        stream = "TTOAT x" + QString::number(_rd) + ", x" + QString::number(_rs2) + ", x" + QString::number(_rs1);
                        return RVInstr::TTOAT;
                    case 0b0000010:
                        stream = "DELAY x" + QString::number(_rs1);
                        return RVInstr::DELAY;
                    default:
                        break;
                    }
                    break;
                }
                case 0b010: {
                    const Word funct7 = IDEX::get_digits(instr, 31, 25);
                    switch (funct7) {
                    case 0b0000000:
                        stream = "TKEND";
                        return RVInstr::ADDTK;
                    case 0b0000001:
                        stream = "ADDTK x" + QString::number(_rs1) + ", x" + QString::number(_rs2);
                        return RVInstr::ADDTK;
                    default:
                        break;
                    }
                    break;
                }
                default:
                    break;
                }
                break;
            }
            default:
                break;
            }
            // Fallthrough - unknown instruction.
            stream = "ILLEGAL Instr.";
            return RVInstr::NOP;
        }();
        if (IDEX::get_digits(instr, 6, 0) == 0b0110011) {
            stream = stream + "x" + QString::number(_rd) + ", x" + QString::number(_rs1) + ", x" + QString::number(_rs2);
        } else {
            [=, &stream] {
                switch(_opcode) {
                case RVInstr::LUI:
                case RVInstr::AUIPC:
                    stream = stream + "x" + QString::number(_rd) + ", " + unsigned2qstring(instr & 0xfffff000);
                    return;
                case RVInstr::JAL: {
                    auto imm = (IDEX::get_digits(instr, 30, 21) << 1) |
                                (IDEX::get_digits(instr, 20, 20) << 11) |
                                (IDEX::get_digits(instr, 19, 12) << 12) |
                                IDEX::expand_digit(IDEX::get_digits(instr, 31, 31), 20);
                    stream = stream + "x " + QString::number(_rd) + ", " + unsigned2qstring(imm);
                    return;
                }
                case RVInstr::JALR: {
                    int imm = IDEX::get_digits(instr, 30, 20) | IDEX::expand_digit(IDEX::get_digits(instr, 31, 31), 11);
                    stream = stream + "x" + QString::number(_rd) + ", " + "x" + QString::number(_rs1) + ", " + QString::number(imm);
                    return;
                }
                case RVInstr::BEQ:
                case RVInstr::BNE:
                case RVInstr::BLT:
                case RVInstr::BGE:
                case RVInstr::BLTU:
                case RVInstr::BGEU: {
                    int imm = (IDEX::get_digits(instr, 11, 8) << 1) |
                            (IDEX::get_digits(instr, 30, 25) << 5) |
                            (IDEX::get_digits(instr, 7, 7) << 11) |
                            IDEX::expand_digit(IDEX::get_digits(instr, 31, 31), 12);
                    stream = stream + "x" + QString::number(_rs1) + ", " + "x" + QString::number(_rs2) + ", " + QString::number(imm);
                    return;
                }
                case RVInstr::LB:
                case RVInstr::LH:
                case RVInstr::LW:
                case RVInstr::LBU:
                case RVInstr::LHU: {
                    int imm = IDEX::get_digits(instr, 30, 20) | IDEX::expand_digit(IDEX::get_digits(instr, 31, 31), 11);
                    stream = stream + "x" + QString::number(_rd) + ", " + QString::number(imm) + "(" + "x" + QString::number(_rs1) + ")";
                    return;
                }
                case RVInstr::ADDI:
                case RVInstr::SLTI:
                case RVInstr::SLTIU:
                case RVInstr::XORI:
                case RVInstr::ORI:
                case RVInstr::ANDI:
                case RVInstr::SLLI:
                case RVInstr::SRLI: {
                    int imm = IDEX::get_digits(instr, 30, 20) | IDEX::expand_digit(IDEX::get_digits(instr, 31, 31), 11);
                    stream = stream + "x" + QString::number(_rd) + ", " + "x" + QString::number(_rs1) + ", " + QString::number(imm);
                    return;
                }
                case RVInstr::SRAI: {
                    int imm = IDEX::get_digits(instr, 24, 20) | IDEX::expand_digit(IDEX::get_digits(instr, 31, 31), 5);
                    stream = stream + "x" + QString::number(_rd) + ", " + "x" + QString::number(_rs1) + ", " + QString::number(imm);
                    return;
                }
                case RVInstr::SB:
                case RVInstr::SH:
                case RVInstr::SW: {
                    int imm = IDEX::get_digits(instr, 11, 7) |
                            (IDEX::get_digits(instr, 30, 25) << 5) |
                            IDEX::expand_digit(IDEX::get_digits(instr, 31, 31), 11);
                    stream = stream + "x" + QString::number(_rs2) + ", " + QString::number(imm) + "(" + "x" + QString::number(_rs1) + ")";
                    return;
                }
                }
            }();
        }
        return stream;
    }
};

#endif // ALGO_H
