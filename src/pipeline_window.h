#ifndef PIPELINE_WINDOW_H
#define PIPELINE_WINDOW_H

#include <QWidget>

class RPU;
class QLabel;
class QStandardItemModel;

class PipelineWindow : public QWidget {
    Q_OBJECT

private:
    // private ui members
    QLabel *thread_id;
    QStandardItemModel *model;
    // private date members
    RPU *rpu;
    // private functions
    void set_item(int row, int col, QString s);
public:
    PipelineWindow(QWidget *parent = nullptr, RPU *r = nullptr);
    void update_rpu(RPU *r);
    void update_stages();
};

#endif // PIPELINE_WINDOW_H
