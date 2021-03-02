#ifndef RPU_SIMULATOR_MEMORYACCESS_H
#define RPU_SIMULATOR_MEMORYACCESS_H

#include "../include/Common.h"
#include "../include/Register.hpp"

class RPU;

class MMWB {
public:
    RPU *rpu;
    // mm/wb data
    Register npc;
    Register alu_result, mem_result;
    Register rd;
    // mm/wb control
    Register reg_write, wb_src_ctl;
    // this is used to track instructions in pipeline
    Register pc;

    MMWB(RPU *r);
    void hook();
    void tick(bool reset, bool we);
};

#endif
