#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <sudokuSolver.h>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QLabel>
#include "Observer.h"
#include "gridRecognizer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public Observer
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent=0);
    ~MainWindow();
    SudokuSolver* getSudokuSolver(){return &sudoku;}


private:
    Ui::MainWindow *ui;
    QTableWidget grid;
    QTableWidgetItem items[9][9];

    GridRecognizer gridReognizer;
    SudokuSolver sudoku;


    void update();
    void setSudokuSolver(SudokuSolver& _solver);

public slots:
    void loadFile();
    void needHelp();
    void solveSudoku();


};

#endif // MAINWINDOW_H
