#ifndef RPU_SIMULATOR_REGISTER_HPP
#define RPU_SIMULATOR_REGISTER_HPP

#include "Common.h"

class Register {
public:
	Word reset_value;
	Word curr;
	Word next;

    Register(Word rv = 0) : reset_value(rv), curr(rv), next(rv) {}

    Word read() { return curr; }

    void write(const Word &t) {
        next = t;
    }

    void tick(bool reset, bool we) {
        if (reset) {
			curr = next = reset_value;
        } else if (we) {
			curr = next;
        } else {
            next = curr;
        }
    }
};

#endif
