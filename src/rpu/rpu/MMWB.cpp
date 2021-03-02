#include "MMWB.h"
#include "../RPU.h"
#include "EXMM.h"
#include "IDEX.h"
#include "../mm/Memory.hpp"

MMWB::MMWB(RPU *r) : rpu(r), pc(unsigned(int(-1))) {}

void MMWB::hook() {
	if (rpu->exmm->mem_write.read() == 1) {
		Word addr = rpu->exmm->alu_result.read();
		Mask m = { false, false, false, false };
		switch (rpu->exmm->mem_ctrl.read()) {
		case MemOp::SB:
			m[addr % 4] = true;
			rpu->dmem->write(addr - 0x10000000, rpu->exmm->r2.read(), m);
			break;
		case MemOp::SH:
			if ((addr >> 1) % 2 == 1) {
				m[2] = true;
				m[3] = true;
			}
			else {
				{
					m[0] = true;
					m[1] = true;
				}
			}
			rpu->dmem->write(addr - 0x10000000, rpu->exmm->r2.read(), m);
			break;
		case MemOp::SW:
			m[0] = true;
			m[1] = true;
			m[2] = true;
			m[3] = true;
			rpu->dmem->write(addr - 0x10000000, rpu->exmm->r2.read(), m);
			break;
		default:
			break;
		}
	}
    npc.write(rpu->exmm->npc.read());
    alu_result.write(rpu->exmm->alu_result.read());
    const Word data = rpu->dmem->read(rpu->exmm->alu_result.read() - rpu->dm_start);
    mem_result.write([=] {
        switch (rpu->exmm->mem_ctrl.read()) {
            case MemOp::LB:
                return (IDEX::get_digits(data, 7, 0)) |
                        IDEX::expand_digit(IDEX::get_digits(data, 7, 7), 8);
            case MemOp::LBU:
                return IDEX::get_digits(data, 7, 0);
            case MemOp::LH:
                return (IDEX::get_digits(data, 15, 0)) |
                        IDEX::expand_digit(IDEX::get_digits(data, 15, 15), 16);
            case MemOp::LHU:
                return IDEX::get_digits(data, 15, 0);
            case MemOp::LW:
                return data;
            default:
                return data;
        }
    }());
    rd.write(rpu->exmm->rd.read());
    reg_write.write(rpu->exmm->reg_write.read());
    wb_src_ctl.write(rpu->exmm->wb_src_ctl.read());
    pc.write(rpu->exmm->pc.read());
}

void MMWB::tick(bool reset, bool we) {
    npc.tick(reset, we);
    alu_result.tick(reset, we);
    mem_result.tick(reset, we);
    rd.tick(reset, we);
    reg_write.tick(reset, we);
    wb_src_ctl.tick(reset, we);
    pc.tick(reset, we);
}
