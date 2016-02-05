#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QGraphicsEllipseItem>
#include <iostream>
#include <QTableWidgetItem>

MainWindow::MainWindow(SudokuSolver& solver, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    sudoku(solver)
{
    ui->setupUi(this);
    grid = new QTableWidget(this);
    grid->setColumnCount(9);
    grid->setRowCount(9);
    grid->setGeometry(20,20,460,460);
    grid->horizontalHeader()->hide();
    grid->verticalHeader()->hide();
    for(int i = 0; i < 9; ++i)
    {
        grid->setRowHeight(i,50);
        grid->setColumnWidth(i, 50);
    }
    for(int i = 0; i < 9; ++i)
    {
        for(int j = 0; j < 9; ++j)
        {
            items[i][j].setTextAlignment(Qt::AlignCenter);
            items[i][j].setText(QString::fromStdString(std::to_string(sudoku.getValue(i,j))));
            grid->setItem(i, j, &items[i][j]);
        }
    }
    helpButton.setParent(this);
    helpButton.setText("HELP");
    helpButton.setGeometry(580,25,50,50);
    QObject::connect(&helpButton, SIGNAL(clicked()), this, SLOT(needHelp())) ;

    solve.setParent(this);
    solve.setText("Solve");
    solve.setGeometry(580,80,50,50);
    QObject::connect(&solve, SIGNAL(clicked()), this, SLOT(solveSudoku())) ;



    end.setParent(this);
    end.setGeometry(500,200,50,100);
    end.setText("SOLVED");
    end.setVisible(false);
}

void MainWindow::needHelp()
{
    sudoku.hideExceptOne();
    update();
}

void MainWindow::solveSudoku()
{
    sudoku.isValid(0);
    update();
}

void MainWindow::update()
{
    for(int i = 0; i < 9; ++i)
    {
        for(int j = 0; j < 9; ++j)
        {
            items[i][j].setText(QString::fromStdString(std::to_string(sudoku.getValue(i, j))));
        }
    }
    if(sudoku.isFilled())
        end.setVisible(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}
