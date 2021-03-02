#ifndef RPU_SIMULATOR_HAZARD_H
#define RPU_SIMULATOR_HAZARD_H

#include "../include/Common.h"
#include "../RPU.h"

class Hazard {
public:
    RPU *rpu;

    Hazard(RPU *r);
    bool isStall();
};

#endif
