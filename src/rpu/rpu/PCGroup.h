#ifndef RPU_SIMULATOR_PCGROUP_H
#define RPU_SIMULATOR_PCGROUP_H

#include "../include/Common.h"
#include "../include/Register.hpp"
#include "../RPU.h"

class RPU;

class PCGroup {
public:
    RPU *rpu;
    Register *pg[RPU::thread_num];
    Register id;

    PCGroup(RPU *r);
    ~PCGroup();
    void hook();
    void tick(bool reset, bool we);
};

#endif
