#ifndef RPU_SIMULATOR_EXECUTE_H
#define RPU_SIMULATOR_EXECUTE_H

#include "../include/Common.h"
#include "../include/Register.hpp"

class RPU;

class EXMM {
private:
    Register timestamp;
    Word get_alu_result();
public:
    RPU *rpu;
    // ex/mm data
    // used by JAL(R), link pc+4
    Register npc;
    Register alu_result, r2;
    Register rd;
    // ex/mm control
    Register reg_write, wb_src_ctl;
    Register mem_write, mem_ctrl;
    // this is used to track instructions in pipeline
    Register pc;

    EXMM(RPU *r);
    void hook();
    void tick(bool reset, bool we);

    bool timed_stall();
    bool get_pc_src_ctl();
    Word get_pc_des();
    Word get_timestamp();
};

#endif
