#include "PCGroup.h"
#include "ThreadControl.h"

PCGroup::PCGroup(RPU *r) : rpu(r), id(0) {
    for (int i = 0; i < RPU::thread_num; i++) {
        pg[i] = new Register(RPU::boot_pc);
    }
}

PCGroup::~PCGroup() {
    for (int i = 0; i < RPU::thread_num; i++) {
        delete pg[i];
    }
}

void PCGroup::hook() {
    id.write(rpu->tc->state.read() == SwitchStatus::WID ? rpu->tc->queue[rpu->tc->front][1] : id.read());
};

void PCGroup::tick(bool reset, bool we) {
    if (reset) {
        for (int i = 0; i < RPU::thread_num; i++) {
            pg[i]->tick(true, false);
        }
    }
    id.tick(reset, we);
}
