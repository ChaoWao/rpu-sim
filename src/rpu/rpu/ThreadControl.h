#ifndef RPU_SIMULATOR_THREADCONTROL_H
#define RPU_SIMULATOR_THREADCONTROL_H

#include <vector>
#include "../include/Common.h"
#include "../include/Register.hpp"

using std::vector;

class RPU;

class ThreadControl {
private:
    bool pop;
    bool isFull();
public:
    static const int MAX_ENTRY = 20;
    bool isEmpty();
    RPU *rpu;
    vector<vector<unsigned>> queue;
    int front;
    int end;
    Register state;

    ThreadControl(RPU *r);
    void hook();
    void tick(bool reset, bool we);
};

#endif
