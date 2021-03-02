#ifndef CODE_WINDOW_H
#define CODE_WINDOW_H

#include <QWidget>

class RPU;
class QLabel;
class QStandardItemModel;

class CodeWindow : public QWidget {
    Q_OBJECT
private:
    // private ui members
    QLabel *program_path_label;
    QStandardItemModel *model;
    // private date members
    int if_index;
    int id_index;
    int ex_index;
    int mm_index;
    int wb_index;
    RPU *rpu;
    // private functions
    void set_item(int row, int col, QString s);

public:
    // Program location
    CodeWindow(QWidget *parent = nullptr, RPU *r = nullptr);
    void update_rpu(RPU *r);
    void update_instr_memory(const QString &path = tr(""));
    void update_stage_label();
};

#endif // CODE_WINDOW_H
