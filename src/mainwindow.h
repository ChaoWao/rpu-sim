#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class RPU;
class CMTWindow;
class CycleWindow;
class RegisterWindow;
class MemoryWindow;
//class BreakpointWindow;
class CodeWindow;
class PipelineWindow;
//class StatisticsWindow;
QT_BEGIN_NAMESPACE
class QMenuBar;
class QMenu;
class QAction;
class QMdiArea;
class QMdiSubWindow;
class QString;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void reset_cpu_action_triggered();
    void reset_all_action_triggered();
    void load_program_action_triggered();
    void exit_action_triggered();
    void run_single_cycle_action_triggered();
    void run_multi_cycle_action_triggered();
    void tile_action_triggered();
    void cascade_action_triggered();
    void unfold_action_triggered();
    void fold_action_triggered();
    void show_cycle_window_action_triggered();
    void show_register_window_action_triggered();
    void show_memory_window_action_triggered();
    void show_code_window_action_triggered();
    void show_pipeline_window_action_triggered();
    void show_cmt_window_action_triggered();

public:
    // RPU
    RPU *rpu;
private:
    // Menu bar begins
//	QMenuBar *menu_bar;
    // Menus of Menu bar
    QMenu *file_menu;
    QMenu *run_menu;
    // TO-DO
//    QMenu *memory_menu;
//    QMenu *code_menu;
//    QMenu *config_menu;
    QMenu *windows_menu;
//    QMenu *help_menu;
    // Actions of each Menu
    // Actions of file_menu
    QAction *reset_cpu_action;
    QAction *reset_all_action;
    QAction *load_program_action;
    QAction *exit_action;
    // Actions of execution_menu
    QAction *run_single_cycle_action;
//    QAction *undo_last_cycle_action;
    QAction *run_multi_cycle_action;
//    QAction *run_continuously_action;
//    QAction *run_to_action;
    // Action of windows_menu
    QAction *tile_action;
    QAction *cascade_action;
    QAction *unfold_action;
    QAction *fold_action;
    QAction *show_cycle_window_action;
    QAction *show_register_window_action;
    QAction *show_memory_window_action;
//    QAction *show_breakpoint_window_action;
    QAction *show_code_window_action;
    QAction *show_pipeline_window_action;
    QAction *show_cmt_window_action;
//    QAction *show_statistics_window_action;
    // Menu bar ends

    // Mdi area begins
	QMdiArea *mdi_area;
    CycleWindow *cycle_window;
    RegisterWindow *register_window;
    MemoryWindow *memory_window;
    CodeWindow *code_window;
    PipelineWindow *pipeline_window;
    CMTWindow *cmt_window;
    // TO-DO
//    QStatusBar *status_bar;

    // function used to build ui
    void create_actions();
    void load_windows();
    void update_ui();
    void update_contents();
    void load_sub_window(QWidget *);
    void active_sub_window(QWidget *);

};

#endif
