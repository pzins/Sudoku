#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "sudokuSolver.h"
#include "gridRecognizer.h"
using namespace cv;

/*
 * chiffres1 fonctionnent maintenant
 * chiffres et fonctionnent pas
 * voir faire pls tailles de base différentes pr chaque filtre chiffre et faire somme des similarity
 *
 * revoir l'ajout de x /3 et y / 3 ds compute pr prendre en compte les traits + gros de la grille
 *
 * enlever les constantes 9 pr nimporte quelle taille de départ
 */


int main()
{
    GridRecognizer gr("s5.png");
    Grid g(gr.getGrid());
    cv::waitKey(0);
    gr.exportToFile("export.txt");


    SudokuSolver s;
    s.import9x9FromFile("export.txt");
    s.print();

    //s.importGrid(grid);
    //s.initQueue();
    //s.print();
    //boost::timer t;
    //s.megaOptIsValid(s.nextVoidCase());
    s.isValid(0);
    // s.optIsValid(0);
    s.print();
    //std::cout << t.elapsed() << std::endl;



    return 0;
}
