#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "sudokuSolver.h"
#include "gridRecognizer.h"
#include <QApplication>
#include "mainwindow.h"
using namespace cv;

/*
 * chiffres1 fonctionnent maintenant
 * chiffres et fonctionnent pas
 * voir faire pls tailles de base différentes pr chaque filtre chiffre et faire somme des similarity
 *
 * revoir l'ajout de x /3 et y / 3 ds compute pr prendre en compte les traits + gros de la grille
 *
 * enlever les constantes 9 pr nimporte quelle taille de départ
 * ajouter une fct qui test la validité d'un sudoku pr afficher erreur avant tte action
 */
/*
 * dernier TRUC:  probleme encore pour utiliser TESSERACT!!!!!!!!!
 * */

int main(int argc, char *argv[])
{
//    GridRecognizer gr("s5.png", 9);
//    Grid g(gr.getGrid());
//    gr.exportToFile("export.txt");



    QApplication a(argc, argv);

    MainWindow win;
    win.getSudokuSolver()->addObserver(win);
    //win.getSudokuSolver()->importGridFromFile("export.txt");

    //s.importGrid(grid);
    //s.initQueue();
    //s.print();
    //boost::timer t;
    //s.megaOptIsValid(s.nextVoidCase());
    //s.isValid(0);
    //s.hideExceptOne();
    //s.print();
    // s.optIsValid(0);
    //std::cout << t.elapsed() << std::endl;


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
  on a les bons trucs ds le XXXX sinon par default ds /usr/localeconv()
**/








