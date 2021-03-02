#ifndef RPU_SIMULATOR_REGISTERFILE_HPP
#define RPU_SIMULATOR_REGISTERFILE_HPP

#include <cstring>
#include "../include/Common.h"

class RegisterFile {
    static const int REG_NUM = 32;
public:
    Word curr[REG_NUM];
    Word next[REG_NUM];

    RegisterFile() {
        memset(curr, 0, sizeof(curr));
        memset(next, 0, sizeof(next));
    }

    void tick(bool reset, bool we) {
        if (reset) {
            memset(curr, 0, sizeof(curr));
            memset(next, 0, sizeof(next));
        } else {
            if (we) {
                memcpy(curr, next, sizeof(curr));
            } else {
                memcpy(next, curr, sizeof(curr));
            }
        }
    }

    Word read(int id) { return id == 0 ? 0 : curr[id]; }
    void write(int id, Word val) { if (id != 0) next[id] = val; }
};

#endif
