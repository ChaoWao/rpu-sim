#include "pipeline_window.h"
#include "rpu/include/Common.h"
#include "rpu/include/Register.hpp"
#include "rpu/RPU.h"
#include "rpu/mm/Memory.hpp"
#include "rpu/rpu/IFID.h"
#include "rpu/rpu/IDEX.h"
#include "rpu/rpu/EXMM.h"
#include "rpu/rpu/MMWB.h"
#include "rpu/rpu/PCGroup.h"
#include "algo.h"

#include <QMdiSubWindow>
#include <QMdiArea>
#include <QTableView>
#include <QLabel>
#include <QString>
#include <QStringList>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QHeaderView>

PipelineWindow::PipelineWindow(QWidget *parent, RPU *r) : QWidget(parent), rpu(r) {
    // init Widget
    setWindowTitle(tr("Pipeline"));
    auto layout = new QVBoxLayout(this);
    // set thread id
    thread_id = new QLabel();
    thread_id->setText(tr(""));
    layout->addWidget(thread_id);
    auto table_view = new QTableView(this);
    model = new QStandardItemModel;
    model->setColumnCount(4);
    model->setRowCount(11);
    set_item(1, 1, tr("IF"));
    set_item(3, 1, tr("ID"));
    set_item(5, 1, tr("EX"));
    set_item(7, 1, tr("MM"));
    set_item(9, 1, tr("WB"));
    table_view->setModel(model);
    table_view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table_view->verticalHeader()->hide();
    table_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table_view->horizontalHeader()->hide();
    table_view->setShowGrid(false);
    layout->addWidget(table_view);
    if (nullptr != rpu) {
        update_stages();
    }
    setLayout(layout);
}

void PipelineWindow::update_rpu(RPU *r) {
    if (rpu != r) {
        rpu = r;
        update_stages();
    }
}

void PipelineWindow::update_stages() {
    thread_id->setText(tr(""));
    set_item(1, 2, tr(""));
    set_item(3, 2, tr(""));
    set_item(5, 2, tr(""));
    set_item(7, 2, tr(""));
    set_item(9, 2, tr(""));
    if (nullptr == rpu) {
        return;
    }
    // refresh this cycle
    thread_id->setText(Algo::int2qstring(rpu->pcg->id.read()));
    int if_pc = int(rpu->pc->read());
    if (if_pc != -1) {
        set_item(1, 2, Algo::instr2assemble(rpu->imem->read(if_pc)));
    }
    int id_pc = int(rpu->ifid->pc.read());
    if (id_pc != -1) {
        set_item(3, 2, Algo::instr2assemble(rpu->imem->read(id_pc)));
    }
    int ex_pc = int(rpu->idex->pc.read());
    if (ex_pc != -1) {
        set_item(5, 2, Algo::instr2assemble(rpu->imem->read(ex_pc)));
    }
    int mm_pc = int(rpu->exmm->pc.read());
    if (mm_pc != -1) {
        set_item(7, 2, Algo::instr2assemble(rpu->imem->read(mm_pc)));
    }
    int wb_pc = int(rpu->mmwb->pc.read());
    if (wb_pc != -1) {
        set_item(9, 2, Algo::instr2assemble(rpu->imem->read(wb_pc)));
    }
}


void PipelineWindow::set_item(int row, int col, QString s)
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
