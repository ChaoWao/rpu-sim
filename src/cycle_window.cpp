#include "cycle_window.h"
#include "mainwindow.h"
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

CycleWindow::CycleWindow(QWidget *parent, RPU *r) : QWidget(parent), rpu(r) {
    // init Widget
    setWindowTitle(tr("Cycle Diagram"));
    auto layout = new QVBoxLayout(this);
    auto table_view = new QTableView(this);
    model = new QStandardItemModel;
    table_view->setModel(model);
    layout->addWidget(table_view);
    if (nullptr != rpu) {
        update_diagram();
    }
    setLayout(layout);
}

void CycleWindow::update_rpu(RPU *r) {
    if (rpu != r) {
        clear_diagram();
        rpu = r;
        update_diagram();
    }
}

void CycleWindow::update_diagram() {
    int col = model->columnCount();
    int wb_pc = int(rpu->mmwb->pc.read());
    if (wb_pc != -1) {
        QString wb_qstr = "T" + Algo::int2qstring(rpu->pcg->id.read()) + "." + Algo::instr2assemble(rpu->imem->read(wb_pc));
        for (int row = model->rowCount()-1; row >= 0 && row >= model->rowCount()-5; row--) {
            if (model->verticalHeaderItem(row)->text() == wb_qstr) {
                set_item(row, col, tr("WB"));
            }
        }
    }
    int mm_pc = int(rpu->exmm->pc.read());
    if (mm_pc != -1) {
        QString mm_qstr = "T" + Algo::int2qstring(rpu->pcg->id.read()) + "." + Algo::instr2assemble(rpu->imem->read(mm_pc));
        for (int row = model->rowCount()-1; row >= 0 && row >= model->rowCount()-5; row--) {
            if (model->verticalHeaderItem(row)->text() == mm_qstr) {
                set_item(row, col, tr("MM"));
            }
        }
    }
    int ex_pc = int(rpu->idex->pc.read());
    if (ex_pc != -1) {
        QString ex_qstr = "T" + Algo::int2qstring(rpu->pcg->id.read()) + "." + Algo::instr2assemble(rpu->imem->read(ex_pc));
        for (int row = model->rowCount()-1; row >= 0 && row >= model->rowCount()-5; row--) {
            if (model->verticalHeaderItem(row)->text() == ex_qstr) {
                set_item(row, col, tr("EX"));
            }
        }
    }
    int id_pc = int(rpu->ifid->pc.read());
    if (id_pc != -1) {
        QString id_qstr = "T" + Algo::int2qstring(rpu->pcg->id.read()) + "." + Algo::instr2assemble(rpu->imem->read(id_pc));
        for (int row = model->rowCount()-1; row >= 0 && row >= model->rowCount()-5; row--) {
            if (model->verticalHeaderItem(row)->text() == id_qstr) {
                set_item(row, col, tr("ID"));
            }
        }
    }
    int if_pc = int(rpu->pc->read());
    if (if_pc != -1) {
        QString if_qstr = "T" + Algo::int2qstring(rpu->pcg->id.read()) + "." + Algo::instr2assemble(rpu->imem->read(if_pc));
        if (model->rowCount() == 0 || model->verticalHeaderItem(model->rowCount()-1)->text() != if_qstr) {
            model->setVerticalHeaderItem(model->rowCount(), new QStandardItem(if_qstr));
        }
        set_item(model->rowCount()-1, col, tr("IF"));
    }
}

void CycleWindow::clear_diagram() {
    model->clear();
}

void CycleWindow::set_item(int row, int col, QString s)
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
