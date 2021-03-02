#ifndef MEMORY_WINDOW_H
#define MEMORY_WINDOW_H

#include <QWidget>

class RPU;
class QStandardItemModel;

class MemoryWindow : public QWidget {
    Q_OBJECT
private:
    // private ui members
    QStandardItemModel *model;
    // private date members
    RPU *rpu;
    // private functions
    void set_item(int row, int col, QString s);

public:
    MemoryWindow(QWidget *parent = nullptr, RPU *r = nullptr);
    void update_rpu(RPU *r);
    void update_data_memory();
};

#endif // CODE_WINDOW_H
