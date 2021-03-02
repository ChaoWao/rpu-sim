#include "mainwindow.h"
#include "register_window.h"
#include "memory_window.h"
#include "code_window.h"
#include "pipeline_window.h"
#include "cycle_window.h"
#include "cmt_window.h"

#include <QApplication>
#include <QMenuBar>
#include <QMenu>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QString>
#include <QFileDialog>
#include <QInputDialog>
#include <QDebug>
#include <QLineEdit>
#include <QDir>

#include "rpu/include/Register.hpp"
#include "rpu/RPU.h"
#include "rpu/rpu/IFID.h"
#include "rpu/rpu/IDEX.h"
#include "rpu/rpu/EXMM.h"
#include "rpu/rpu/MMWB.h"
#include "rpu/mm/Memory.hpp"
#include "algo.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    rpu = new RPU();
    create_actions();
    mdi_area = new QMdiArea(this);
    mdi_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdi_area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdi_area);
    load_windows();
    update_ui();
}

MainWindow::~MainWindow()
{
    delete rpu;
}

void MainWindow::create_actions()
{
    // file menu
    file_menu = menuBar()->addMenu(tr("&File"));
    reset_cpu_action = new QAction(tr("Reset &CPU"));
    connect(reset_cpu_action, &QAction::triggered, this, &MainWindow::reset_cpu_action_triggered);
    file_menu->addAction(reset_cpu_action);
    reset_all_action = new QAction(tr("Reset &All"));
    connect(reset_all_action, &QAction::triggered, this, &MainWindow::reset_all_action_triggered);
    file_menu->addAction(reset_all_action);
    load_program_action = new QAction(tr("&Load Program"));
    connect(load_program_action, &QAction::triggered, this, &MainWindow::load_program_action_triggered);
    file_menu->addAction(load_program_action);
    file_menu->addSeparator();
    exit_action = new QAction(tr("E&xit"));
    connect(exit_action, &QAction::triggered, this, &MainWindow::exit_action_triggered);
    file_menu->addAction(exit_action);
    // run menu
    run_menu = menuBar()->addMenu(tr("&Run"));
    run_single_cycle_action = new QAction(tr("Run &Single Cycle"));
    connect(run_single_cycle_action, &QAction::triggered, this, &MainWindow::run_single_cycle_action_triggered);
    run_menu->addAction(run_single_cycle_action);
    run_multi_cycle_action = new QAction(tr("Run &Multi Cycle"));
    connect(run_multi_cycle_action, &QAction::triggered, this, &MainWindow::run_multi_cycle_action_triggered);
    run_menu->addAction(run_multi_cycle_action);
    // windows menu
    windows_menu = menuBar()->addMenu(tr("&Windows"));
    tile_action = new QAction(tr("&Tile"));
    connect(tile_action, &QAction::triggered, this, &MainWindow::tile_action_triggered);
    windows_menu->addAction(tile_action);
    cascade_action = new QAction(tr("&Cascade"));
    connect(cascade_action, &QAction::triggered, this, &MainWindow::cascade_action_triggered);
    windows_menu->addAction(cascade_action);
    unfold_action = new QAction(tr("&Unfold"));
    connect(unfold_action, &QAction::triggered, this, &MainWindow::unfold_action_triggered);
    windows_menu->addAction(unfold_action);
    fold_action = new QAction(tr("&Fold"));
    connect(fold_action, &QAction::triggered, this, &MainWindow::fold_action_triggered);
    windows_menu->addAction(fold_action);
    windows_menu->addSeparator();
    show_cycle_window_action = new QAction(tr("Cycle &Diagram"));
    connect(show_cycle_window_action, &QAction::triggered, this, &MainWindow::show_cycle_window_action_triggered);
    windows_menu->addAction(show_cycle_window_action);
    show_register_window_action = new QAction(tr("&Register"));
    connect(show_register_window_action, &QAction::triggered, this, &MainWindow::show_register_window_action_triggered);
    windows_menu->addAction(show_register_window_action);
    show_memory_window_action = new QAction(tr("&Memory"));
    connect(show_memory_window_action, &QAction::triggered, this, &MainWindow::show_memory_window_action_triggered);
    windows_menu->addAction(show_memory_window_action);
    show_code_window_action = new QAction(tr("&Code"));
    connect(show_code_window_action, &QAction::triggered, this, &MainWindow::show_code_window_action_triggered);
    windows_menu->addAction(show_code_window_action);
    show_pipeline_window_action = new QAction(tr("&Pipeline"));
    connect(show_pipeline_window_action, &QAction::triggered, this, &MainWindow::show_pipeline_window_action_triggered);
    windows_menu->addAction(show_pipeline_window_action);
    show_cmt_window_action = new QAction(tr("CM&T"));
    connect(show_cmt_window_action, &QAction::triggered, this, &MainWindow::show_cmt_window_action_triggered);
    windows_menu->addAction(show_cmt_window_action);
}

void MainWindow::load_windows() {
    cycle_window = new CycleWindow(nullptr, rpu);
    load_sub_window(cycle_window);
    register_window = new RegisterWindow(nullptr, rpu);
    load_sub_window(register_window);
    memory_window = new MemoryWindow(nullptr, rpu);
    load_sub_window(memory_window);
    code_window = new CodeWindow(nullptr, rpu);
    load_sub_window(code_window);
    pipeline_window = new PipelineWindow(nullptr, rpu);
    load_sub_window(pipeline_window);
    cmt_window = new CMTWindow(nullptr, rpu);
    load_sub_window(cmt_window);
}

void MainWindow::update_ui() {
    showMaximized();
    tile_action_triggered();
}

void MainWindow::update_contents() {
    cycle_window->update_diagram();
    register_window->update_register();
    memory_window->update_data_memory();
    code_window->update_stage_label();
    pipeline_window->update_stages();
    cmt_window->update_tt_table();
}

void MainWindow::load_sub_window(QWidget *w)
{
    auto sw = mdi_area->addSubWindow(w);
    sw->setWindowTitle(w->windowTitle());
    sw->setWindowIcon(w->windowIcon());
}

void MainWindow::active_sub_window(QWidget *w)
{
    auto sw = qobject_cast<QMdiSubWindow *>(w->parentWidget());
    mdi_area->setActiveSubWindow(sw);
}

// private slots start here
void MainWindow::reset_cpu_action_triggered() {
    rpu->reset();
    cycle_window->clear_diagram();
    update_contents();
}

void MainWindow::reset_all_action_triggered() {
    delete rpu;
    rpu = new RPU();
    cycle_window->update_rpu(rpu);
    register_window->update_rpu(rpu);
    memory_window->update_rpu(rpu);
    code_window->update_rpu(rpu);
    pipeline_window->update_rpu(rpu);
    cmt_window->update_rpu(rpu);
}

void MainWindow::exit_action_triggered() {
    QApplication::quit();
}

void MainWindow::load_program_action_triggered()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Load Program", ".", "Hex File (*.hex)");
    rpu->load_hex_imem(file_name.toStdString().c_str());
    code_window->update_instr_memory(file_name);
    file_name.replace("imem", "dmem");
    rpu->load_hex_dmem(file_name.toStdString().c_str());
    reset_cpu_action_triggered();
}

void MainWindow::run_single_cycle_action_triggered()
{
    rpu->tick();
    update_contents();
}

void MainWindow::run_multi_cycle_action_triggered()
{
    bool ok;
    int num = QInputDialog::getInt(this, tr("Run Multi Cycles"), tr("Number of cycles to run:"), 1, 0, 2147483647, 1, &ok);
    if (ok) {
        while (num-- > 0) {
            rpu->tick();
            update_contents();
        }
    }
}

void MainWindow::tile_action_triggered()
{
    cycle_window->parentWidget()->showNormal();
    memory_window->parentWidget()->showNormal();
    pipeline_window->parentWidget()->showNormal();
    register_window->parentWidget()->showNormal();
    code_window->parentWidget()->showNormal();
    cmt_window->parentWidget()->showNormal();
//    swStatistics->parentWidget()->showNormal();
//    swBreakpoint->parentWidget()->showMinimized();
    mdi_area->tileSubWindows();
}

void MainWindow::cascade_action_triggered()
{
    cycle_window->parentWidget()->showNormal();
    memory_window->parentWidget()->showNormal();
    pipeline_window->parentWidget()->showNormal();
    register_window->parentWidget()->showNormal();
    code_window->parentWidget()->showNormal();
    cmt_window->parentWidget()->showNormal();
    mdi_area->cascadeSubWindows();
}

void MainWindow::unfold_action_triggered()
{
    tile_action_triggered();
}

void MainWindow::fold_action_triggered()
{
    cycle_window->parentWidget()->showMinimized();
    memory_window->parentWidget()->showMinimized();
    pipeline_window->parentWidget()->showMinimized();
    register_window->parentWidget()->showMinimized();
    code_window->parentWidget()->showMinimized();
    cmt_window->parentWidget()->showMinimized();
}

void MainWindow::show_cycle_window_action_triggered()
{
    active_sub_window(cycle_window);
}

void MainWindow::show_register_window_action_triggered()
{
    active_sub_window(register_window);
}

void MainWindow::show_memory_window_action_triggered()
{
    active_sub_window(memory_window);
}

void MainWindow::show_pipeline_window_action_triggered()
{
    active_sub_window(pipeline_window);
}

void MainWindow::show_code_window_action_triggered()
{
    active_sub_window(code_window);
}

void MainWindow::show_cmt_window_action_triggered()
{
    active_sub_window(cmt_window);
}

