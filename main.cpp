#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "sudokuSolver.h"
#include "gridRecognizer.h"
#include <QApplication>
#include "mainwindow.h"

using namespace cv;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow win;
    win.getSudokuSolver()->addObserver(win);

    win.show();
    return a.exec();

}


/**
  tesseract a besoin de libtenocia
  la dernière version de tessercat a besoin d'une version recente de libtenocia + que clle ds apt-get
  dc j'ai recup leptonica 1.73 => compilé ds download + make install dc ds system normalement (cmake puis make install)

  tesseract : ./autogen => crée le configure
  ./configure --prefix=XXXX
  make install
  on a les bons trucs ds le XXXX sinon par default ds /usr/locale conv()
**/








