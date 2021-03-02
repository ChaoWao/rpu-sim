#ifndef RPU_SIMULATOR_TU_H
#define RPU_SIMULATOR_TU_H

#include "../include/Common.h"
#include "../include/Register.hpp"

class RPU;

class TU {
private:
    RPU *rpu;
    Register time;
    Register cnt;
public:
    Register tg;
    TU(RPU *r);
    Word get_time();
    void hook();
    void std_clk_tick(bool reset, bool we);
};

#endif
