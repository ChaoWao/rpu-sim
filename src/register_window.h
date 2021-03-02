#ifndef REGISTER_WINDOW_H
#define REGISTER_WINDOW_H

#include <QWidget>
#include <QString>
#include <vector>

class RPU;
class QTableView;
class QStandardItemModel;
class QRadioButton;

class RegisterWindow : public QWidget {
    Q_OBJECT
private:
    //
    const std::vector<QString> stage_reg_names = {"PC", "IF/ID.nPC", "IF/ID.PC", "IF/ID.IR", "ID/EX.nPC", "ID/EX.PC",\
                                                 "ID/EX.R1", "ID/EX.R2", "ID/EX.IMM", "ID/EX.RD", "ID/EX.RS1", "ID/EX.RS2",\
                                                 "ID/EX.OPCODE", "ID/EX.REG_WRITE", "ID/EX.WB_SRC_CTL", "ID/EX.MEM_WRITE",\
                                                 "ID/EX.MEM_CTRL", "ID/EX.JUMP", "ID/EX.BRANCH", "ID/EX.ALU_OP1_CTL",\
                                                 "ID/EX.ALU_OP2_CTL", "ID/EX.MEM_READ", "ID/EX.ALU_OP", "ID/EX.BRANCH_OP",\
                                                 "EX/MM.nPC", "EX/MM.ALU_RESULT", "EX/MM.R2", "EX/MM.RD", "EX/MM.REG_WRITE",\
                                                 "EX/MM.WB_SRC_CTL", "EX/MM.MEM_WRITE", "EX/MM.MEM_CTRL",\
                                                 "MM/WB.nPC", "MM/WB.ALU_RESULT", "MM/WB.MEM_RESULT", "MM/WB.RD",\
                                                 "MM/WB.REG_WRITE", "MM/WB.WB_SRC_CTL",\
                                                 "TG", "TIME", "TIMESTAMP"};
    // private ui members
    QRadioButton *regfile_deci_rb;
    QRadioButton *regfile_hex_rb;
    QTableView *regfile_table_view;
    QStandardItemModel *regfile_model;
    QRadioButton *stage_reg_deci_rb;
    QRadioButton *stage_reg_hex_rb;
    QTableView *stage_reg_table_view;
    QStandardItemModel *stage_reg_model;
    // private date members
    RPU *rpu;
    //
    void set_item(QStandardItemModel *model, int row, int col, QString s);
private slots:
    void regfile_deci_rb_clicked();
    void regfile_hex_rb_clicked();
    void stage_reg_deci_rb_clicked();
    void stage_reg_hex_rb_clicked();
public:
    RegisterWindow(QWidget *parent = nullptr, RPU *r = nullptr);
    void update_rpu(RPU *r);
    void update_register();
};

#endif // REGISTER_WINDOW_H
