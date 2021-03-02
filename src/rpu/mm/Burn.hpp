#ifndef RPU_SIMULATOR_BURN_HPP
#define RPU_SIMULATOR_BURN_HPP

#include <iomanip>
#include <sstream>
#include <string>
#include "Memory.hpp"
#include "../include/Common.h"

class Burn {
public:
    static Word hex2Word(const std::string &hex) {
        return std::stoul(hex.c_str(), nullptr, 16);
    }

    static void burn_hex(std::istream &in, Memory &mem) {
        Addr base_addr = 0;
        std::string hex;
        Mask mask = {true, true, true, true};
        while(in >> hex) {
            mem.write(base_addr, hex2Word(hex), mask);
            base_addr += 4;
        }
    }
};


#endif
