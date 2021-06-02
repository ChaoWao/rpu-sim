#include "RPU.h"
#include "rpu/IFID.h"
#include "rpu/IDEX.h"
#include "rpu/EXMM.h"
#include "rpu/MMWB.h"
#include "rpu/Hazard.h"
#include "rpu/TU.h"
#include "include/Register.hpp"
#include "rpu/RegisterFile.hpp"
#include "rpu/PCGroup.h"
#include "rpu/RegisterFileGroup.h"
#include "rpu/ThreadControl.h"
#include "mm/Memory.hpp"
#include "mm/Burn.hpp"

#include <fstream>
//#include <random>

RPU::RPU() :
    if_ss(StageStatus::RESET),
    id_ss(StageStatus::RESET),
    ex_ss(StageStatus::RESET),
    mm_ss(StageStatus::RESET),
    wb_ss(StageStatus::RESET)
{
    // mem and register
    imem = new Memory();
    dmem = new Memory();
    rfg = new RegisterFileGroup(this);
    rf = rfg->rfg[0];
    // pc and stage register
    pcg = new PCGroup(this);
    pc = pcg->pg[0];
    ifid = new IFID(this);
    idex = new IDEX(this);
    exmm = new EXMM(this);
    mmwb = new MMWB(this);
    tu = new TU(this);
    tc = new ThreadControl(this);
    // comb logics
    h = new Hazard(this);
}

RPU::~RPU() {
    delete h;
    delete tc;
    delete tu;
    delete mmwb;
    delete exmm;
    delete idex;
    delete ifid;
    delete pcg;
    delete rfg;
    delete imem;
    delete dmem;
}

void RPU::reset() {
    // reset all timing logics
    tc->tick(true, false);
    tu->std_clk_tick(true, false);
    pcg->tick(true, false);
    pc = pcg->pg[0];
    rfg->tick(true, false);
    rf = rfg->rfg[0];
    ifid->tick(true, false);
    idex->tick(true, false);
    exmm->tick(true, false);
    mmwb->tick(true, false);
}

void RPU::load_hex_imem(const char *path) {
    std::fstream in(path);
    Burn::burn_hex(in, *imem);
}

void RPU::load_hex_dmem(const char *path) {
    std::fstream in(path);
    Burn::burn_hex(in, *dmem);
}

void RPU::tick() {
    // hook stage register
    tc->hook();
    mmwb->hook();
    exmm->hook();
    tu->hook();
    idex->hook();
    ifid->hook();
    pcg->hook();
    rfg->hook();
    // hook pc
    if (tc->state.read() == SwitchStatus::WID) {
        pc = pcg->pg[tc->queue[tc->front][1]];
    }
    if (tc->state.read() == SwitchStatus::WBPC) {
        pc->write((idex->pc.read() != unsigned(-1) ? idex->pc.read() : (ifid->pc.read() != unsigned(-1) ? ifid->pc.read() : pc->read())));
    } else if (exmm->get_pc_src_ctl() == PcSrc::ALU) {
        pc->write(exmm->get_pc_des());
    } else {
        pc->write(pc->read()+4);
    }
    // hook register file
    if (tc->state.read() == SwitchStatus::WID) {
        rf = rfg->rfg[tc->queue[tc->front][1]];
    }
    switch (mmwb->wb_src_ctl.read()) {
    case RegWrSrc::ALURES:     // OP(IMM) : write alu result
        rf->write(mmwb->rd.read(), mmwb->alu_result.read());
        break;
    case RegWrSrc::MEMREAD:     // LOAD : write mem result
        rf->write(mmwb->rd.read(), mmwb->mem_result.read());
        break;
    case RegWrSrc::PC4:     // JAL(R) : write npc
        rf->write(mmwb->rd.read(), mmwb->npc.read());
        break;
    default:
        break;
    }

    // detect hazard
    bool time_hazard = exmm->timed_stall();
    bool data_hazard = h->isStall();
	bool control_hazard = exmm->get_pc_src_ctl() == PcSrc::ALU;
    // tick to sync
    rf->tick(false, mmwb->reg_write.read() == 1);
    if (tc->state.read() == SwitchStatus::WID) {
        pc->tick(false, false);
        ifid->tick(false, false);
        idex->tick(false, false);
        exmm->tick(false, false);
        mmwb->tick(false, false);
    }
    else if (tc->state.read() == SwitchStatus::WBREG) {
        pc->tick(false, false);
        ifid->tick(false, false);
        idex->tick(false, false);
        exmm->tick(false, false);
        mmwb->tick(true, false);
    }
    else if (tc->state.read() == SwitchStatus::WBPC) {
        pc->tick(false, true);
        ifid->tick(true, false);
        idex->tick(true, false);
        exmm->tick(true, false);
        mmwb->tick(false, true);
    }
    else if (time_hazard) {
        pc->tick(false, false);
        ifid->tick(false, false);
        idex->tick(false, false);
        exmm->tick(true, false);
        mmwb->tick(false, true);
        wb_ss = mm_ss;
        mm_ss = StageStatus::FLUSH;
        ex_ss = StageStatus::STALL;
        id_ss = StageStatus::STALL;
        if_ss = StageStatus::STALL;
    }
    else if (control_hazard) {
		pc->tick(false, true);
		ifid->tick(true, false);
		idex->tick(true, false);
		exmm->tick(false, true);
        mmwb->tick(false, true);
        wb_ss = mm_ss;
        mm_ss = ex_ss;
        ex_ss = StageStatus::FLUSH;
        id_ss = StageStatus::FLUSH;
        if_ss = StageStatus::RUN;
	}
    else {
        pc->tick(false, !data_hazard);
		ifid->tick(false, !data_hazard);
		idex->tick(data_hazard, true);
		exmm->tick(false, true);
        mmwb->tick(false, true);
        wb_ss = mm_ss;
        mm_ss = ex_ss;
        ex_ss = data_hazard ? Word(StageStatus::FLUSH) : id_ss;
        id_ss = data_hazard ? Word(StageStatus::STALL) : if_ss;
        if_ss = data_hazard ? StageStatus::STALL : StageStatus::RUN;
	}
    pcg->tick(false, true);
    rfg->tick(false, true);
    tc->tick(false, true);
    if (cnt == cycle_per_us) {
        tu->std_clk_tick(false, true);
        cnt = 1;
    } else {
        cnt++;
    }
}
