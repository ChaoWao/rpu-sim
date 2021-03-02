#ifndef RPU_SIMULATOR_RPU_HPP
#define RPU_SIMULATOR_RPU_HPP

#include "include/Common.h"

class Register;
class Memory;
class RegisterFile;
class PCGroup;
class RegisterFileGroup;
class IFID;
class IDEX;
class EXMM;
class MMWB;
class Hazard;
class TU;
class ThreadControl;

class RPU {
private:
    static const int cycle_per_us = 50;
    int cnt = 0;
public:
    static const int thread_num = 4;
	// instruction memory start at 0x00000000
	// boot address
	static const Addr boot_pc = 0x00000000;
	// data memory start at 0x00000000
	static const Addr dm_start = 0x10000000;
    // instruction and data memory
    Memory *imem;
    Memory *dmem;
    // register file
    RegisterFile *rf;
    RegisterFileGroup *rfg;
    // pc
    Register *pc;
    PCGroup *pcg;
    // pipeline stages
    IFID *ifid;
    IDEX *idex;
    EXMM *exmm;
    MMWB *mmwb;
    TU *tu;
    ThreadControl *tc;
    // comb logics
    Hazard *h;
    // stage status indicate variable
    Word if_ss;
    Word id_ss;
    Word ex_ss;
    Word mm_ss;
    Word wb_ss;

    RPU();
    ~RPU();
    void load_hex_imem(const char *path);
    void load_hex_dmem(const char *path);
    void tick();
    void reset();
};

#endif
