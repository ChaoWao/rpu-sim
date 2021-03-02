#include "TU.h"
#include "../RPU.h"
#include "../rpu/IDEX.h"

TU::TU(RPU *r) : rpu(r), time(0), cnt(0), tg(0) {}

Word TU::get_time() {
    return time.read();
}

void TU::hook() {
    if (tg.read() == 0) {
        cnt.write(0);
        time.write(time.read());
    } else {
        if (cnt.read() >= tg.read()-1) {
            time.write(time.read()+1);
            cnt.write(0);
        } else {
            time.write(time.read());
            cnt.write(cnt.read()+1);
        }
    }
    if (rpu->idex->opcode.read() == RVInstr::SETTG) {
        tg.write(rpu->idex->r1.read());
        tg.tick(false, true);
    } else if (rpu->idex->opcode.read() == RVInstr::SETTI) {
        time.write(rpu->idex->r1.read());
        time.tick(false, true);
        cnt.write(1);
    }
}

void TU::std_clk_tick(bool reset, bool we) {
    if (reset) {
        tg.tick(reset, we);
    }
    time.tick(reset, we);
    cnt.tick(reset, we);
}
