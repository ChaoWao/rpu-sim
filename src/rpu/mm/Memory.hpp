#ifndef RPU_SIMULATOR_MEMORY_HPP
#define RPU_SIMULATOR_MEMORY_HPP

#include <cstring>
#include "../include/Common.h"

class Memory {
public:
    Byte *mem;
    Addr size;
    Memory(const Addr memory_size = MEMORY_SIZE) : size(memory_size) {
        mem = new Byte[size];
        memset(mem, 0, size * sizeof(Byte));
    }

    ~Memory() {
        delete mem;
    }

    Word read(Addr addr) {
        if (addr >= size) return 0;
        // this line handles align
        addr = addr - addr % 4;
        return *((Word *)(mem+addr));
    }

    void write(Addr addr, Word word, Mask mask) {
        // this line handles align
        if (addr >= size) return;
        addr = addr % size;
        addr = addr - addr % 4;
        mem[addr] = mask[0] ? (unsigned char)(word) : mem[addr];
        mem[addr+1] = mask[1] ? (unsigned char)(word >> 8) : mem[addr+1];
        mem[addr+2] = mask[2] ? (unsigned char)(word >> 16) : mem[addr+2];
        mem[addr+3] = mask[3] ? (unsigned char)(word >> 24) : mem[addr+3];
    }
};

#endif
