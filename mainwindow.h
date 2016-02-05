#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <sudokuSolver.h>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    QTableWidget* grid;
    SudokuSolver sudoku;
    QTableWidgetItem items[9][9];
    QPushButton helpButton;
    QPushButton solve;
    QLabel end;

public:
    explicit MainWindow(SudokuSolver& solver, QWidget *parent=0);
    ~MainWindow();
    void update();

public slots:
    void needHelp();
    void solveSudoku();


};

#endif // MAINWINDOW_H
