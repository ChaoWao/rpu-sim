#include "cmt_window.h"
#include "mainwindow.h"
#include "rpu/include/Common.h"
#include "rpu/include/Register.hpp"
#include "rpu/RPU.h"
#include "rpu/mm/Memory.hpp"
#include "rpu/rpu/RegisterFile.hpp"
#include "rpu/rpu/IFID.h"
#include "rpu/rpu/IDEX.h"
#include "rpu/rpu/EXMM.h"
#include "rpu/rpu/MMWB.h"
#include "rpu/rpu/ThreadControl.h"
#include "rpu/rpu/PCGroup.h"
#include "rpu/rpu/RegisterFileGroup.h"
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
#include <QRadioButton>
#include <QGroupBox>

CMTWindow::CMTWindow(QWidget *parent, RPU *r) : QWidget(parent), rpu(r) {
    // init Widget
    setWindowTitle(tr("CMT"));
    auto layout = new QHBoxLayout(this);
    // TT table ui
    auto table_view = new QTableView(this);
    table_view->verticalHeader()->hide();
    tt_table_model = new QStandardItemModel;
    QStringList headers;
    headers << tr("Time") << tr("Hardware Thread ID");
    tt_table_model->setHorizontalHeaderLabels(headers);
    table_view->setModel(tt_table_model);
    layout->addWidget(table_view);
    // Multi thread ui
    auto vlayout = new QVBoxLayout();
    auto thread_gb = new QGroupBox();
    auto thread_gb_layout = new QHBoxLayout();
    for (int i = 0; i < RPU::thread_num; i++) {
        thread_rb[i] = new QRadioButton(tr("T") + Algo::int2qstring(i));
        connect(thread_rb[i], &QRadioButton::clicked, this, &CMTWindow::rb_clicked);
        thread_gb_layout->addWidget(thread_rb[i]);
    }
    thread_rb[0]->setChecked(true);
    thread_gb->setLayout(thread_gb_layout);
    vlayout->addWidget(thread_gb);
    auto gb = new QGroupBox();
    auto gb_layout = new QHBoxLayout();
    deci_rb = new QRadioButton(tr("Decimal"));
    connect(deci_rb, &QRadioButton::clicked, this, &CMTWindow::rb_clicked);
    gb_layout->addWidget(deci_rb);
    hex_rb = new QRadioButton(tr("Hex"));
    connect(hex_rb, &QRadioButton::clicked, this, &CMTWindow::rb_clicked);
    gb_layout->addWidget(hex_rb);
    deci_rb->setChecked(true);
    gb->setLayout(gb_layout);
    vlayout->addWidget(gb);
    auto thread_table_view = new QTableView(this);
    thread_table_view->horizontalHeader()->hide();
    thread_table_view->verticalHeader()->hide();
    thread_model = new QStandardItemModel;
    thread_model->setColumnCount(2);
    thread_model->setRowCount(33);
    set_item(thread_model, 0, 0, tr("PC"));
    for (auto i = 1; i < thread_model->rowCount(); i++) {
        QString qstr = tr("x") + Algo::int2qstring(i-1);
        set_item(thread_model, i, 0, qstr);
    }
    thread_table_view->setModel(thread_model);
    vlayout->addWidget(thread_table_view);
    layout->addLayout(vlayout);
    if (nullptr != rpu) {
        update_tt_table();
    }
    setLayout(layout);
}

void CMTWindow::update_rpu(RPU *r) {
    if (rpu != r) {
        rpu = r;
        update_tt_table();
    }
}

void CMTWindow::update_tt_table() {
    if (nullptr != rpu) {
        if (!rpu->tc->isEmpty()) {
            int j = 0;
            for (int i = rpu->tc->front; i != rpu->tc->end; i = (i+1)%ThreadControl::MAX_ENTRY) {
                set_item(tt_table_model, j, 0, Algo::int2qstring(rpu->tc->queue[i][0]));
                set_item(tt_table_model, j, 1, Algo::int2qstring(rpu->tc->queue[i][1]));
                j++;
            }
            tt_table_model->setRowCount(j);
        } else {
            tt_table_model->setRowCount(0);
        }
        rb_clicked();
    }
}

void CMTWindow::set_item(QStandardItemModel *model, int row, int col, QString s)
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

void CMTWindow::rb_clicked() {
    int i = 0;
    for (; i < RPU::thread_num; i++) {
        if (thread_rb[i]->isChecked()) {
            break;
        }
    }
    i = i == RPU::thread_num ? 0 : i;
    if (deci_rb->isChecked()) {
        set_item(thread_model, 0, 1, Algo::int2qstring(rpu->pcg->pg[i]->read()));
        for (auto j = 1; j < thread_model->rowCount(); j++) {
            set_item(thread_model, j, 1, Algo::int2qstring(rpu->rfg->rfg[i]->read(j-1)));
        }
    } else {
        set_item(thread_model, 0, 1, Algo::unsigned2qstring(rpu->pcg->pg[i]->read()));
        for (auto j = 1; j < thread_model->rowCount(); j++) {
            set_item(thread_model, j, 1, Algo::unsigned2qstring(rpu->rfg->rfg[i]->read(j-1)));
        }
    }
}
