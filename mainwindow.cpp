#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QMainWindow>
#include <QGraphicsEllipseItem>
#include <iostream>
#include <QTableWidgetItem>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->verticalLayout->insertWidget(0, &grid);

    grid.setColumnCount(9);
    grid.setRowCount(9);
    grid.horizontalHeader()->hide();
    grid.verticalHeader()->hide();
    for(int i = 0; i < 9; ++i)
    {
        grid.setRowHeight(i,50);
        grid.setColumnWidth(i, 50);
    }

    QObject::connect(ui->helpButton, SIGNAL(clicked()), this, SLOT(needHelp())) ;
    QObject::connect(ui->solveButton, SIGNAL(clicked()), this, SLOT(solveSudoku())) ;
    QObject::connect(ui->fileButton, SIGNAL(clicked()), this, SLOT(loadFile())) ;


    ui->resultLabel->setVisible(false);
    ui->resultLabel->setText("SOLVED");

}

void MainWindow::update()
{
    for(int i = 0; i < 9; ++i)
    {
        for(int j = 0; j < 9; ++j)
        {
            items[i][j].setTextAlignment(Qt::AlignCenter);
            items[i][j].setText(QString::fromStdString(std::to_string(sudoku.getValue(j, i))));
        }
    }
    if(sudoku.isFilled())
        ui->resultLabel->setVisible(true);

}

void MainWindow::setSudokuSolver(SudokuSolver &_solver)
{
    sudoku = _solver;
}


MainWindow::~MainWindow()
{
    delete ui;
}


/** =========== SLOTS ============= **/
void MainWindow::needHelp()
{
    sudoku.hideExceptOne();
}

void MainWindow::solveSudoku()
{
    sudoku.solve();
}

void MainWindow::loadFile()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("Images (*.png *.jpg)");

    if (dialog.exec())
    {
        std::string filename = dialog.selectedFiles().at(0).toStdString();
        gridReognizer.setFilename(filename);
        gridReognizer.extractGrid();

        gridReognizer.exportToFile("export");
        sudoku.importGridFromFile("export");
        QImage image(filename.c_str());

        ui->imageLabel->setPixmap(QPixmap::fromImage(image.scaled(250,250)));
    }

}

