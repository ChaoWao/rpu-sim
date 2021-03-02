#include "register_window.h"
#include "rpu/include/Common.h"
#include "rpu/include/Register.hpp"
#include "rpu/RPU.h"
#include "rpu/mm/Memory.hpp"
#include "rpu/rpu/IFID.h"
#include "rpu/rpu/IDEX.h"
#include "rpu/rpu/EXMM.h"
#include "rpu/rpu/MMWB.h"
#include "rpu/rpu/RegisterFile.hpp"
#include "rpu/rpu/TU.h"
#include "algo.h"

#include <QMdiSubWindow>
#include <QMdiArea>
#include <QTableView>
#include <QLabel>
#include <QString>
#include <QStringList>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QRadioButton>
#include <QGroupBox>

RegisterWindow::RegisterWindow(QWidget *parent, RPU *r) : QWidget(parent), rpu(r) {
    // init Widget
    setWindowTitle(tr("Register"));
    // init ui
    auto h_layout = new QHBoxLayout(this);
    // regfile
    auto regfile_layout = new QVBoxLayout();
    auto regfile_gb = new QGroupBox();
    auto regfile_rb_layout = new QHBoxLayout();
    regfile_deci_rb = new QRadioButton("Decimal");
    connect(regfile_deci_rb, &QRadioButton::clicked, this, &RegisterWindow::regfile_deci_rb_clicked);
    regfile_rb_layout->addWidget(regfile_deci_rb);
    regfile_hex_rb = new QRadioButton("Hex");
    connect(regfile_hex_rb, &QRadioButton::clicked, this, &RegisterWindow::regfile_hex_rb_clicked);
    regfile_rb_layout->addWidget(regfile_hex_rb);
    regfile_gb->setLayout(regfile_rb_layout);
    regfile_layout->addWidget(regfile_gb);
    regfile_table_view = new QTableView();
    regfile_table_view->verticalHeader()->hide();
    regfile_table_view->horizontalHeader()->hide();
    regfile_model = new QStandardItemModel;
    regfile_model->setColumnCount(2);
    regfile_model->setRowCount(32);
    for (auto i = 0; i < regfile_model->rowCount(); i++) {
        QString qstr = tr("x") + Algo::int2qstring(i);
        set_item(regfile_model, i, 0, qstr);
    }
    regfile_table_view->setModel(regfile_model);
    regfile_layout->addWidget(regfile_table_view);
    h_layout->addLayout(regfile_layout);
    // stage reg
    auto stage_reg_layout = new QVBoxLayout();
    auto stage_reg_gb = new QGroupBox();
    auto stage_reg_rb_layout = new QHBoxLayout();
    stage_reg_deci_rb = new QRadioButton("Decimal");
    connect(stage_reg_deci_rb, &QRadioButton::clicked, this, &RegisterWindow::stage_reg_deci_rb_clicked);
    stage_reg_rb_layout->addWidget(stage_reg_deci_rb);
    stage_reg_hex_rb = new QRadioButton("Hex");
    connect(stage_reg_hex_rb, &QRadioButton::clicked, this, &RegisterWindow::stage_reg_hex_rb_clicked);
    stage_reg_rb_layout->addWidget(stage_reg_hex_rb);
    stage_reg_gb->setLayout(stage_reg_rb_layout);
    stage_reg_layout->addWidget(stage_reg_gb);
    stage_reg_table_view = new QTableView();
    stage_reg_table_view->verticalHeader()->hide();
    stage_reg_table_view->horizontalHeader()->hide();
    stage_reg_model = new QStandardItemModel;
    stage_reg_model->setColumnCount(2);
    stage_reg_model->setRowCount(stage_reg_names.size());
    for (auto i = 0; i < stage_reg_model->rowCount(); i++) {
        set_item(stage_reg_model, i, 0, stage_reg_names[i]);
    }
    stage_reg_table_view->setModel(stage_reg_model);
    stage_reg_layout->addWidget(stage_reg_table_view);
    h_layout->addLayout(stage_reg_layout);
    // init show type
    regfile_deci_rb_clicked();
    stage_reg_deci_rb_clicked();
    // update
    if (nullptr != rpu) {
        update_register();
    }
    setLayout(h_layout);
}

void RegisterWindow::update_rpu(RPU *r) {
    if (rpu != r) {
        rpu = r;
        update_register();
    }
}

void RegisterWindow::update_register() {
    if (nullptr == rpu) {
        for (auto i = 0; i < regfile_model->rowCount(); i++) {
            set_item(regfile_model, i, 1, tr(""));
        }
        for (auto i = 0; i < stage_reg_model->rowCount(); i++) {
            set_item(stage_reg_model, i, 1, tr(""));
        }
        return;
    }
    for (auto i = 0; i < regfile_model->rowCount(); i++) {
        if (regfile_deci_rb->isChecked()) {
            set_item(regfile_model, i, 1, Algo::int2qstring(rpu->rf->read(i)));
        } else {
            set_item(regfile_model, i, 1, Algo::unsigned2qstring(rpu->rf->read(i)));
        }
    }
    if (stage_reg_deci_rb->isChecked()) {
        set_item(stage_reg_model, 0, 1, Algo::int2qstring(rpu->pc->read()));
        set_item(stage_reg_model, 1, 1, Algo::int2qstring(rpu->ifid->npc.read()));
        set_item(stage_reg_model, 2, 1, Algo::int2qstring(rpu->ifid->pc.read()));
        set_item(stage_reg_model, 3, 1, Algo::int2qstring(rpu->ifid->instr.read()));
        set_item(stage_reg_model, 4, 1, Algo::int2qstring(rpu->idex->npc.read()));
        set_item(stage_reg_model, 5, 1, Algo::int2qstring(rpu->idex->pc.read()));
        set_item(stage_reg_model, 6, 1, Algo::int2qstring(rpu->idex->r1.read()));
        set_item(stage_reg_model, 7, 1, Algo::int2qstring(rpu->idex->r2.read()));
        set_item(stage_reg_model, 8, 1, Algo::int2qstring(rpu->idex->imm.read()));
        set_item(stage_reg_model, 9, 1, Algo::int2qstring(rpu->idex->rd.read()));
        set_item(stage_reg_model, 10, 1, Algo::int2qstring(rpu->idex->rs1.read()));
        set_item(stage_reg_model, 11, 1, Algo::int2qstring(rpu->idex->rs2.read()));
        set_item(stage_reg_model, 12, 1, Algo::int2qstring(rpu->idex->opcode.read()));
        set_item(stage_reg_model, 13, 1, Algo::int2qstring(rpu->idex->reg_write.read()));
        set_item(stage_reg_model, 14, 1, Algo::int2qstring(rpu->idex->wb_src_ctl.read()));
        set_item(stage_reg_model, 15, 1, Algo::int2qstring(rpu->idex->mem_write.read()));
        set_item(stage_reg_model, 16, 1, Algo::int2qstring(rpu->idex->mem_ctrl.read()));
        set_item(stage_reg_model, 17, 1, Algo::int2qstring(rpu->idex->jump.read()));
        set_item(stage_reg_model, 18, 1, Algo::int2qstring(rpu->idex->branch.read()));
        set_item(stage_reg_model, 19, 1, Algo::int2qstring(rpu->idex->alu_op1_ctl.read()));
        set_item(stage_reg_model, 20, 1, Algo::int2qstring(rpu->idex->alu_op2_ctl.read()));
        set_item(stage_reg_model, 21, 1, Algo::int2qstring(rpu->idex->mem_read.read()));
        set_item(stage_reg_model, 22, 1, Algo::int2qstring(rpu->idex->alu_op.read()));
        set_item(stage_reg_model, 23, 1, Algo::int2qstring(rpu->idex->branch_op.read()));
        set_item(stage_reg_model, 24, 1, Algo::int2qstring(rpu->exmm->npc.read()));
        set_item(stage_reg_model, 25, 1, Algo::int2qstring(rpu->exmm->alu_result.read()));
        set_item(stage_reg_model, 26, 1, Algo::int2qstring(rpu->exmm->r2.read()));
        set_item(stage_reg_model, 27, 1, Algo::int2qstring(rpu->exmm->rd.read()));
        set_item(stage_reg_model, 28, 1, Algo::int2qstring(rpu->exmm->reg_write.read()));
        set_item(stage_reg_model, 29, 1, Algo::int2qstring(rpu->exmm->wb_src_ctl.read()));
        set_item(stage_reg_model, 30, 1, Algo::int2qstring(rpu->exmm->mem_write.read()));
        set_item(stage_reg_model, 31, 1, Algo::int2qstring(rpu->exmm->mem_ctrl.read()));
        set_item(stage_reg_model, 32, 1, Algo::int2qstring(rpu->mmwb->npc.read()));
        set_item(stage_reg_model, 33, 1, Algo::int2qstring(rpu->mmwb->alu_result.read()));
        set_item(stage_reg_model, 34, 1, Algo::int2qstring(rpu->mmwb->mem_result.read()));
        set_item(stage_reg_model, 35, 1, Algo::int2qstring(rpu->mmwb->rd.read()));
        set_item(stage_reg_model, 36, 1, Algo::int2qstring(rpu->mmwb->reg_write.read()));
        set_item(stage_reg_model, 37, 1, Algo::int2qstring(rpu->mmwb->wb_src_ctl.read()));
        set_item(stage_reg_model, 38, 1, Algo::int2qstring(rpu->tu->tg.read()));
        set_item(stage_reg_model, 39, 1, Algo::int2qstring(rpu->tu->get_time()));
        set_item(stage_reg_model, 40, 1, Algo::int2qstring(rpu->exmm->get_timestamp()));
    } else {
        set_item(stage_reg_model, 0, 1, Algo::unsigned2qstring(rpu->pc->read()));
        set_item(stage_reg_model, 1, 1, Algo::unsigned2qstring(rpu->ifid->npc.read()));
        set_item(stage_reg_model, 2, 1, Algo::unsigned2qstring(rpu->ifid->pc.read()));
        set_item(stage_reg_model, 3, 1, Algo::unsigned2qstring(rpu->ifid->instr.read()));
        set_item(stage_reg_model, 4, 1, Algo::unsigned2qstring(rpu->idex->npc.read()));
        set_item(stage_reg_model, 5, 1, Algo::unsigned2qstring(rpu->idex->pc.read()));
        set_item(stage_reg_model, 6, 1, Algo::unsigned2qstring(rpu->idex->r1.read()));
        set_item(stage_reg_model, 7, 1, Algo::unsigned2qstring(rpu->idex->r2.read()));
        set_item(stage_reg_model, 8, 1, Algo::unsigned2qstring(rpu->idex->imm.read()));
        set_item(stage_reg_model, 9, 1, Algo::unsigned2qstring(rpu->idex->rd.read()));
        set_item(stage_reg_model, 10, 1, Algo::unsigned2qstring(rpu->idex->rs1.read()));
        set_item(stage_reg_model, 11, 1, Algo::unsigned2qstring(rpu->idex->rs2.read()));
        set_item(stage_reg_model, 12, 1, Algo::unsigned2qstring(rpu->idex->opcode.read()));
        set_item(stage_reg_model, 13, 1, Algo::unsigned2qstring(rpu->idex->reg_write.read()));
        set_item(stage_reg_model, 14, 1, Algo::unsigned2qstring(rpu->idex->wb_src_ctl.read()));
        set_item(stage_reg_model, 15, 1, Algo::unsigned2qstring(rpu->idex->mem_write.read()));
        set_item(stage_reg_model, 16, 1, Algo::unsigned2qstring(rpu->idex->mem_ctrl.read()));
        set_item(stage_reg_model, 17, 1, Algo::unsigned2qstring(rpu->idex->jump.read()));
        set_item(stage_reg_model, 18, 1, Algo::unsigned2qstring(rpu->idex->branch.read()));
        set_item(stage_reg_model, 19, 1, Algo::unsigned2qstring(rpu->idex->alu_op1_ctl.read()));
        set_item(stage_reg_model, 20, 1, Algo::unsigned2qstring(rpu->idex->alu_op2_ctl.read()));
        set_item(stage_reg_model, 21, 1, Algo::unsigned2qstring(rpu->idex->mem_read.read()));
        set_item(stage_reg_model, 22, 1, Algo::unsigned2qstring(rpu->idex->alu_op.read()));
        set_item(stage_reg_model, 23, 1, Algo::unsigned2qstring(rpu->idex->branch_op.read()));
        set_item(stage_reg_model, 24, 1, Algo::unsigned2qstring(rpu->exmm->npc.read()));
        set_item(stage_reg_model, 25, 1, Algo::unsigned2qstring(rpu->exmm->alu_result.read()));
        set_item(stage_reg_model, 26, 1, Algo::unsigned2qstring(rpu->exmm->r2.read()));
        set_item(stage_reg_model, 27, 1, Algo::unsigned2qstring(rpu->exmm->rd.read()));
        set_item(stage_reg_model, 28, 1, Algo::unsigned2qstring(rpu->exmm->reg_write.read()));
        set_item(stage_reg_model, 29, 1, Algo::unsigned2qstring(rpu->exmm->wb_src_ctl.read()));
        set_item(stage_reg_model, 30, 1, Algo::unsigned2qstring(rpu->exmm->mem_write.read()));
        set_item(stage_reg_model, 31, 1, Algo::unsigned2qstring(rpu->exmm->mem_ctrl.read()));
        set_item(stage_reg_model, 32, 1, Algo::unsigned2qstring(rpu->mmwb->npc.read()));
        set_item(stage_reg_model, 33, 1, Algo::unsigned2qstring(rpu->mmwb->alu_result.read()));
        set_item(stage_reg_model, 34, 1, Algo::unsigned2qstring(rpu->mmwb->mem_result.read()));
        set_item(stage_reg_model, 35, 1, Algo::unsigned2qstring(rpu->mmwb->rd.read()));
        set_item(stage_reg_model, 36, 1, Algo::unsigned2qstring(rpu->mmwb->reg_write.read()));
        set_item(stage_reg_model, 37, 1, Algo::unsigned2qstring(rpu->mmwb->wb_src_ctl.read()));
        set_item(stage_reg_model, 38, 1, Algo::unsigned2qstring(rpu->tu->tg.read()));
        set_item(stage_reg_model, 39, 1, Algo::unsigned2qstring(rpu->tu->get_time()));
        set_item(stage_reg_model, 40, 1, Algo::unsigned2qstring(rpu->exmm->get_timestamp()));
    }
    regfile_table_view->resizeColumnsToContents();
    stage_reg_table_view->resizeColumnsToContents();
}


void RegisterWindow::set_item(QStandardItemModel *model, int row, int col, QString s)
{
    auto item = model->takeItem(row, col);
    if (item != nullptr) {
        item->setText(s);
        model->setItem(row, col, item);
    } else {
        item = new QStandardItem();
        item->setText(s);
        model->setItem(row, col, item);
    }
}

// private slots here
void RegisterWindow::regfile_deci_rb_clicked() {
//    regfile_hex_rb->setChecked(false);
    regfile_deci_rb->setChecked(true);
    update_register();
}

void RegisterWindow::regfile_hex_rb_clicked() {
//    regfile_deci_rb->setChecked(false);
    regfile_hex_rb->setChecked(true);
    update_register();
}

void RegisterWindow::stage_reg_deci_rb_clicked() {
//    stage_reg_hex_rb->setChecked(false);
    stage_reg_deci_rb->setChecked(true);
    update_register();
}

void RegisterWindow::stage_reg_hex_rb_clicked() {
//    stage_reg_deci_rb->setChecked(false);
    stage_reg_hex_rb->setChecked(true);
    update_register();
}
