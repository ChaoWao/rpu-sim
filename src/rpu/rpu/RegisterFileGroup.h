#ifndef RPU_SIMULATOR_RFGROUP_H
#define RPU_SIMULATOR_RFGROUP_H

#include "../include/Common.h"
#include "../include/Register.hpp"
#include "../RPU.h"

class RPU;
class RegisterFile;

class RegisterFileGroup {
public:
    RPU *rpu;
    RegisterFile *rfg[RPU::thread_num];
    Register id;

    RegisterFileGroup(RPU *r);
    ~RegisterFileGroup();
    void hook();
    void tick(bool reset, bool we);
};

#endif
