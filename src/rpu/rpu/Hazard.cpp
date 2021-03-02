#include "Hazard.h"
#include "../RPU.h"
#include "IDEX.h"
#include "EXMM.h"
#include "MMWB.h"

Hazard::Hazard(RPU *r) : rpu(r) {}

bool Hazard::isStall() {
    Word rs1 = rpu->idex->_rs1;
    Word rs2 = rpu->idex->_rs2;
    Word ex_reg_write = rpu->idex->reg_write.read();
    Word ex_rd = rpu->idex->rd.read();
    Word mm_reg_write = rpu->exmm->reg_write.read();
    Word mm_rd = rpu->exmm->rd.read();
    Word wb_reg_write = rpu->mmwb->reg_write.read();
    Word wb_rd = rpu->mmwb->rd.read();
    if (ex_reg_write == 1 && ex_rd != 0 && (rs1 == ex_rd || rs2 == ex_rd)) {
        return true;
    }
    if (mm_reg_write == 1 && mm_rd != 0 && (rs1 == mm_rd || rs2 == mm_rd)) {
        return true;
    }
    if (wb_reg_write == 1 && wb_rd != 0 && (rs1 == wb_rd || rs2 == wb_rd)) {
        return true;
    }
    return false;
}