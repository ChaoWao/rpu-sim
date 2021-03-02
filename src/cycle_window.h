#ifndef CYCLE_WINDOW_H
#define CYCLE_WINDOW_H

#include <QWidget>

class RPU;
class QStandardItemModel;

class CycleWindow : public QWidget {
    Q_OBJECT
private:
    // private ui members
    QStandardItemModel *model;
    // private date members
    RPU *rpu;
    // private functions
    void set_item(int row, int col, QString s);

public:
    CycleWindow(QWidget *parent = nullptr, RPU *r = nullptr);
    void update_rpu(RPU *r);
    void clear_diagram();
    void update_diagram();
};

#endif // CYCLE_WINDOW_H
