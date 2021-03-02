#ifndef RPU_SIMULATOR_FETCH_H
#define RPU_SIMULATOR_FETCH_H

#include "../include/Common.h"
#include "../include/Register.hpp"

class RPU;

class IFID {
public:
    RPU *rpu;
    // if/id data
    // used by JAL(R), link pc+4
    Register npc;
    // used by AUIPC, JAL(R), BRANCH
    Register pc;
    Register instr;
    // if/id control
    // none
    // this is used to track instructions in pipeline
    // pc above

    IFID(RPU *r);
    void hook();
    void tick(bool reset, bool we);
};

#endif
