#include "memory_window.h"
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

MemoryWindow::MemoryWindow(QWidget *parent, RPU *r) : QWidget(parent), rpu(r) {
    // init Widget
    setWindowTitle(tr("Memory"));
    auto layout = new QVBoxLayout(this);
    auto table_view = new QTableView(this);
    table_view->verticalHeader()->hide();
    table_view->horizontalHeader()->hide();
    model = new QStandardItemModel;
    table_view->setModel(model);
    layout->addWidget(table_view);
    if (nullptr != rpu) {
        update_data_memory();
    }
    setLayout(layout);
}

void MemoryWindow::update_rpu(RPU *r) {
    if (rpu != r) {
        rpu = r;
        update_data_memory();
    }
}

void MemoryWindow::update_data_memory() {
    if (nullptr == rpu) {
        model->clear();
        return;
    }
    if (model->columnCount() < 5) {
        model->setColumnCount(5);
    }
    // get imem and show
    auto dmem = rpu->dmem;
    Addr row = dmem->size;
    if (model->rowCount() < int(row >> 2)) {
        model->setRowCount(row >> 2);
    }
    for (Addr i = 0; i < row; i+=4) {
        Word w = dmem->read(i);
        set_item(i>>2, 0, Algo::unsigned2qstring(i));
        QString s = Algo::unsigned2qstring(w);
        set_item(i>>2, 1, s.mid(8,2));
        set_item(i>>2, 2, s.mid(6,2));
        set_item(i>>2, 3, s.mid(4,2));
        set_item(i>>2, 4, s.mid(2,2));
    }
}


void MemoryWindow::set_item(int row, int col, QString s)
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
