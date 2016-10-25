#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "otsp_solver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_calc_button_clicked();

    void on_extend_button_clicked();

    void on_cell_dclick(int, int);

private:
    Ui::MainWindow *ui;
    void initTable();
    TSP_Table input_to_tsp();
};

#endif // MAINWINDOW_H
