#include "RegisterFileGroup.h"
#include "RegisterFile.hpp"
#include "ThreadControl.h"

RegisterFileGroup::RegisterFileGroup(RPU *r) : rpu(r), id(0) {
    for (int i = 0; i < RPU::thread_num; i++) {
        rfg[i] = new RegisterFile();
    }
}

RegisterFileGroup::~RegisterFileGroup() {
    for (int i = 0; i < RPU::thread_num; i++) {
        delete rfg[i];
    }
}

void RegisterFileGroup::hook() {
    id.write(rpu->tc->state.read() == SwitchStatus::WID ? rpu->tc->queue[rpu->tc->front][1] : id.read());
};

void RegisterFileGroup::tick(bool reset, bool we) {
    if (reset) {
        for (int i = 0; i < RPU::thread_num; i++) {
            rfg[i]->tick(true, false);
        }
    }
    id.tick(reset, we);
}
