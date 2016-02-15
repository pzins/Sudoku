#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QMainWindow>
#include <QGraphicsEllipseItem>
#include <iostream>
#include <QTableWidgetItem>
#include <QFileDialog>

#define ROW_HEIGHT 50
#define COLUMN_WIDTH 50

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    grid.setParent(this);
    grid.setGeometry(10,10,464,464);
    grid.setColumnCount(NB);
    grid.setRowCount(NB);
    grid.horizontalHeader()->hide();
    grid.verticalHeader()->hide();
    for(int i = 0; i < NB; ++i)
    {
        grid.setRowHeight(i,ROW_HEIGHT);
        grid.setColumnWidth(i, COLUMN_WIDTH);
    }

    for(int i = 0; i < NB; i++)
        for(int j = 0; j < NB; ++j)
        {
            grid.setItem(i,j, &items[i][j]);
            items[i][j].setFont(QFont("Arial", 14, QFont::Bold));
        }

    QObject::connect(ui->helpButton, SIGNAL(clicked()), this, SLOT(needHelp())) ;
    QObject::connect(ui->solveButton, SIGNAL(clicked()), this, SLOT(solveSudoku())) ;
    QObject::connect(ui->fileButton, SIGNAL(clicked()), this, SLOT(loadFile())) ;


}

void MainWindow::update()
{
    for(int i = 0; i < NB; ++i)
    {
        for(int j = 0; j < NB; ++j)
        {
            items[i][j].setTextAlignment(Qt::AlignCenter);
            items[i][j].setText(QString::fromStdString(std::to_string(sudoku.getValue(j, i))));
        }
    }
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
        gridRecognizer.setFilename(filename);
        gridRecognizer.extractGrid();

        gridRecognizer.exportToFile("export");
        sudoku.importGridFromFile("export");
        QImage image(filename.c_str());

        ui->imageLabel->setPixmap(QPixmap::fromImage(image.scaled(250,250)));
    }

}

