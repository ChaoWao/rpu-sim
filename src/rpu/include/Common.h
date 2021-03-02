#ifndef RPU_SIMULATOR_COMMON_H
#define RPU_SIMULATOR_COMMON_H

#include "../../external/vsrtl_enum.h"

// memory types
// Little endian memory
constexpr auto MEMORY_SIZE = 0x1000;
using Byte = unsigned char;
using Word = unsigned int;
using Addr = unsigned int;
using Mask = bool [4];

// thread num
constexpr auto THREAD_NUM = 4;

Enum(RVInstr, NOP,
	/* RV32I Base Instruction Set */
	LUI, AUIPC, JAL, JALR, BEQ, BNE, BLT, BGE, BLTU, BGEU, LB, LH, LW, LBU, LHU, SB, SH, SW, ADDI, SLTI, SLTIU, XORI,
    ORI, ANDI, SLLI, SRLI, SRAI, ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND,
    /* RVTT Extention Instruction Set */
    SETTG, SETTI, GETTI, GETTS, TTIAT, TTOAT, DELAY, TKEND, ADDTK);
Enum(ALUOp, NOP, ADD, SUB, MUL, DIV, AND, OR, XOR, SL, SRA, SRL, LUI, LT, LTU, EQ);
Enum(RegWrSrc, MEMREAD, ALURES, PC4);
Enum(AluSrc1, REG1, PC);
Enum(AluSrc2, REG2, IMM);
Enum(CompOp, NOP, EQ, NE, LT, LTU, GE, GEU);
Enum(MemOp, NOP, LB, LH, LW, LBU, LHU, SB, SH, SW);
Enum(PcSrc, PC4 = 0, ALU = 1);
Enum(StageStatus, RESET, RUN, STALL, FLUSH);
Enum(SwitchStatus, RUN, WBPC, WBREG, WID);

#endif
