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
 * ajouter une fct qui test la validité d'un sudoku pr afficher erreur avant tte action
 */
/*
 * dernier TRUC:  probleme encore pour utiliser TESSERACT!!!!!!!!!
 * */

int main()
{
    GridRecognizer gr("s5.png", 9);
    Grid g(gr.getGrid());
    gr.exportToFile("export.txt");


    SudokuSolver s;
    s.importGridFromFile("export.txt");
    s.print();

    //s.importGrid(grid);
    //s.initQueue();
    //s.print();
    //boost::timer t;
    //s.megaOptIsValid(s.nextVoidCase());
    //s.isValid(0);
    s.hideExceptOne();
    // s.optIsValid(0);
    s.print();
    //std::cout << t.elapsed() << std::endl;



    return 0;
}
