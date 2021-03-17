#include "ThreadControl.h"
#include "../RPU.h"
#include "TU.h"
#include "IDEX.h"

ThreadControl::ThreadControl(RPU *r) : pop(false), rpu(r), queue(vector<vector<unsigned>>(MAX_ENTRY, vector<unsigned>(2, 0))), front(0), end(0), state(SwitchStatus::RUN) {
}

bool ThreadControl::isEmpty() {
    if (front == end) return true;
    return false;
}

bool ThreadControl::isFull() {
    if (front == (end + 1) % MAX_ENTRY) return true;
    return false;
}

void ThreadControl::hook() {
    if (!isFull() && rpu->idex->opcode.read() == RVInstr::ADDTK) {
        queue[end][0] = rpu->idex->r1.read();
        queue[end][1] = rpu->idex->r2.read();
        end = (end + 1) % MAX_ENTRY;
    }
    switch (state.read()) {
    case SwitchStatus::RUN: {
        if (!isEmpty() && pop) {
            front = (front + 1) % MAX_ENTRY;
            pop = false;
        }
        if (rpu->idex->opcode.read() == RVInstr::TKEND) {
            front = (front - 1) % MAX_ENTRY;
            queue[front][0] = 0;
            // soft-real time thread
            queue[front][1] = 3;
            state.write(SwitchStatus::WBPC);
        }
        else if ((!isEmpty() && rpu->tu->get_time() >= queue[front][0])) {
            state.write(SwitchStatus::WBPC);
        }
        else {
            state.write(SwitchStatus::RUN);
        }
        break;
    }
    case SwitchStatus::WBPC: {
        state.write(SwitchStatus::WBREG);
        break;
    }
    case SwitchStatus::WBREG: {
        state.write(SwitchStatus::WID);
        break;
    }
    case SwitchStatus::WID: {
        state.write(SwitchStatus::RUN);
        pop = true;
        break;
    }
    default: {
        state.write(SwitchStatus::RUN);
    }
    }
}

void ThreadControl::tick(bool reset, bool we) {
    if (reset) {
        front = 0;
        end = 0;
    }
    state.tick(reset, we);
}
