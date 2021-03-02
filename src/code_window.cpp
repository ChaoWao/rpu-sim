#include "code_window.h"
#include "mainwindow.h"
#include "rpu/include/Common.h"
#include "rpu/include/Register.hpp"
#include "rpu/RPU.h"
#include "rpu/mm/Memory.hpp"
#include "rpu/rpu/IFID.h"
#include "rpu/rpu/IDEX.h"
#include "rpu/rpu/EXMM.h"
#include "rpu/rpu/MMWB.h"
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

CodeWindow::CodeWindow(QWidget *parent, RPU *r) : QWidget(parent), rpu(r) {
    // init Widget
    setWindowTitle(tr("Code"));
    auto layout = new QVBoxLayout(this);
    // set program path
    program_path_label = new QLabel();
    program_path_label->setText(tr(""));
    layout->addWidget(program_path_label);
    auto table_view = new QTableView(this);
    table_view->verticalHeader()->hide();
    model = new QStandardItemModel;
    QStringList headers;
    headers << tr("Address") << tr("Machine Code") << tr("Pipeline Stage") << tr("Assemble");
    model->setHorizontalHeaderLabels(headers);
    table_view->setModel(model);
    table_view->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(table_view);
    if (nullptr != rpu) {
        update_instr_memory();
    }
    setLayout(layout);
    if_index = id_index = ex_index = mm_index = wb_index = -1;
    update_stage_label();
}

void CodeWindow::update_rpu(RPU *r) {
    if (rpu != r) {
        rpu = r;
        update_instr_memory();
        update_stage_label();
    }
}

void CodeWindow::update_instr_memory(const QString &path) {
    program_path_label->setText(path);
    if (nullptr == rpu) {
        model->clear();
        return;
    }
    if (model->columnCount() < 4) {
        model->setColumnCount(4);
    }
    // get imem and show
    auto imem = rpu->imem;
    Addr row = imem->size;
    if (model->rowCount() < int(row >> 2)) {
        model->setRowCount(row >> 2);
    }
    for (Addr i = 0; i < row; i+=4) {
        Word w = imem->read(i);
        set_item(i>>2, 0, Algo::unsigned2qstring(i));
        set_item(i>>2, 1, Algo::unsigned2qstring(w));
        set_item(i>>2, 2, tr(""));
        set_item(i>>2, 3, Algo::instr2assemble(w));
    }
}

void CodeWindow::update_stage_label() {
    if (nullptr != rpu) {
        // clear last cycle information
        if (if_index != -1) {
            set_item(if_index, 2, tr(""));
        }
        if (id_index != -1) {
            set_item(id_index, 2, tr(""));
        }
        if (ex_index != -1) {
            set_item(ex_index, 2, tr(""));
        }
        if (mm_index != -1) {
            set_item(mm_index, 2, tr(""));
        }
        if (wb_index != -1) {
            set_item(wb_index, 2, tr(""));
        }
        // refresh this cycle
        int if_pc = int(rpu->pc->read()) >> 2;
        if (if_pc != -1) {
            set_item(if_pc, 2, tr("IF"));
        }
        if_index = if_pc;
        int id_pc = int(rpu->ifid->pc.read()) >> 2;
        if (id_pc != -1) {
            set_item(id_pc, 2, tr("ID"));
        }
        id_index = id_pc;
        int ex_pc = int(rpu->idex->pc.read()) >> 2;
        if (ex_pc != -1) {
            set_item(ex_pc, 2, tr("EX"));
        }
        ex_index = ex_pc;
        int mm_pc = int(rpu->exmm->pc.read()) >> 2;
        if (mm_pc != -1) {
            set_item(mm_pc, 2, tr("MM"));
        }
        mm_index = mm_pc;
        int wb_pc = int(rpu->mmwb->pc.read()) >> 2;
        if (wb_pc != -1) {
            set_item(wb_pc, 2, tr("WB"));
        }
        wb_index = wb_pc;
    }
}


void CodeWindow::set_item(int row, int col, QString s)
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
