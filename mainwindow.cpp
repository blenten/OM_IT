#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_calc_button_clicked()
{
    OTSP_Solver solver;

    try{
        solver.set_data(input_to_tsp());
        solver.calculate_path();
    }catch(TSP_Exception ex)
    {
        ui->path_output_line->setText(ex.what());
        ui->len_output_line->setText(ex.what());
        return;
    }

    QString result;
    std::string path = solver.path_to_string();

    result = QString::fromUtf8(path.data(), path.size());
    ui->path_output_line->setText(result);

    result = QString::number(solver.getPathLength());
    ui->len_output_line->setText(result);
}


TSP_Table MainWindow::input_to_tsp()
{
    TSP_Table res;
    int t_size = ui->input_table->rowCount();

    res.resize(t_size);
    for(int i=0; i<t_size; i++)
    {
        res[i].resize(t_size);
    }

    for(int i=0; i<t_size; i++)
        for(int j=0; j<t_size; j++)
        {
            if(ui->input_table->item(i, j)==0) throw TSP_Exception("Wrong data!");
            QString val = ui->input_table->item(i, j)->text();
            //TODO: check for appropriate value
            if(val=="M")
            {
                res[i][j] = TSP::M;
            }else if(val=="")
            {
                throw TSP_Exception("Not enough data!");
            }else{
                res[i][j] = val.toDouble();
            }

        }
    return res;
}


void MainWindow::on_extend_button_clicked()
{
    int row, col;
    row = ui->input_table->rowCount();
    col = ui->input_table->columnCount();

    ui->input_table->insertRow(row);
    ui->input_table->insertColumn(col);

    ui->input_table->setItem(row,col, new QTableWidgetItem("M"));
    ui->input_table->item(row,col)->setFlags(ui->input_table->item(row,col)->flags() ^ Qt::ItemIsEditable);
}

void MainWindow::on_cell_dclick(int row, int col)
{
    if(row==col && ui->input_table->rowCount()>2)
    {
        ui->input_table->removeRow(row);
        ui->input_table->removeColumn(col);
    }
}

void MainWindow::initTable()
{
    //TODO: Aligment center
    ui->input_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->input_table->setRowCount(4);
    ui->input_table->setColumnCount(4);
    for(int i=0; i<ui->input_table->rowCount(); i++)
    {
        ui->input_table->setItem(i,i, new QTableWidgetItem("M"));
        ui->input_table->item(i,i)->setFlags(ui->input_table->item(i,i)->flags() ^ Qt::ItemIsEditable);
    }

    ui->input_table->setItem(0,1, new QTableWidgetItem("5"));
    ui->input_table->setItem(0,2, new QTableWidgetItem("11"));
    ui->input_table->setItem(0,3, new QTableWidgetItem("9"));
    ui->input_table->setItem(1,0, new QTableWidgetItem("10"));
    ui->input_table->setItem(1,2, new QTableWidgetItem("8"));
    ui->input_table->setItem(1,3, new QTableWidgetItem("7"));
    ui->input_table->setItem(2,0, new QTableWidgetItem("7"));
    ui->input_table->setItem(2,1, new QTableWidgetItem("14"));
    ui->input_table->setItem(2,3, new QTableWidgetItem("8"));
    ui->input_table->setItem(3,0, new QTableWidgetItem("12"));
    ui->input_table->setItem(3,1, new QTableWidgetItem("6"));
    ui->input_table->setItem(3,2, new QTableWidgetItem("15"));

    connect(ui->input_table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(on_cell_dclick(int, int)));
}
