#ifndef CMT_WINDOW_H
#define CMT_WINDOW_H

#include <QWidget>
#include "rpu/RPU.h"

class RPU;
class QStandardItemModel;
class QRadioButton;

class CMTWindow : public QWidget {
    Q_OBJECT
private:
    // private ui members
    // TT table
    QStandardItemModel *tt_table_model;
    // Thread Regfile and PC
    QRadioButton *thread_rb[RPU::thread_num];
    QRadioButton *deci_rb;
    QRadioButton *hex_rb;
    QStandardItemModel *thread_model;
    // private date members
    RPU *rpu;
    // private functions
    void set_item(QStandardItemModel *, int row, int col, QString s);

private slots:
    void rb_clicked();

public:
    // Program location
    CMTWindow(QWidget *parent = nullptr, RPU *r = nullptr);
    void update_rpu(RPU *r);
    void update_tt_table();
};

#endif // CODE_WINDOW_H
