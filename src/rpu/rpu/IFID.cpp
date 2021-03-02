#include "IFID.h"
#include "../RPU.h"
#include "../mm/Memory.hpp"

IFID::IFID(RPU *r) : rpu(r), pc(unsigned(int(-1))), instr(0x00000013) {}

void IFID::hook() {
    npc.write(rpu->pc->read()+4);
    instr.write(rpu->imem->read(rpu->pc->read()));
    pc.write(rpu->pc->read());
}

void IFID::tick(bool reset, bool we) {
    npc.tick(reset, we);
    instr.tick(reset, we);
    pc.tick(reset, we);
}
