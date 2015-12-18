#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "sudokuSolver.h"
#include "gridRecognizer.h"
using namespace cv;




int main()
{

    GridRecognizer gr("sudoku.png");
    Grid g(gr.getGrid());
    gr.exportToFile("export.txt");



    for(int i = 0; i < 9; ++i)
    {
        for(int j = 0; j < 9; ++j)
        {
            std::cout << g.at(j).at(i) << " ";
        }
        std::cout << std::endl;
    }

    waitKey();




/*
    int grid[SIZE][SIZE] =
    {
        {9,0,0,1,0,0,0,0,5},
        {0,0,5,0,9,0,2,0,1},
        {8,0,0,0,4,0,0,0,0},
        {0,0,0,0,8,0,0,0,0},
        {0,0,0,7,0,0,0,0,0},
        {0,0,0,0,2,6,0,0,9},
        {2,0,0,3,0,0,0,0,6},
        {0,0,0,2,0,0,9,0,0},
        {0,0,1,9,0,4,5,7,0}
    };
    int hard[SIZE][SIZE] =
    {
        {0,5,0,3,0,9,0,0,0},
        {1,0,0,0,0,0,9,0,5},
        {0,0,7,0,8,0,0,0,6},
        {0,2,0,0,0,0,0,4,0},
        {0,0,0,6,7,2,0,0,0},
        {0,3,0,0,0,0,0,2,0},
        {4,0,0,0,5,0,7,0,0},
        {9,0,8,0,0,0,0,0,2},
        {0,0,0,2,0,1,0,6,0}
    };*/
    SudokuSolver s;
    s.import9x9FromFile();
    s.print();
/*
    //s.importGrid(grid);
    s.initQueue();
    s.print();
    boost::timer t;
    s.megaOptIsValid(s.nextVoidCase());
    // s.isValid(0);
    // s.optIsValid(0);
    s.print();
    std::cout << t.elapsed() << std::endl;
*/

    /*
    for(int i  = 0; i < img.size().height * img.size().width; ++i)
    {
        int p00;
        p00 = img.at<uchar>(i);
        std::cout << p00;
    }*/
    /*
    for(int i = 0; i < img.size().height; ++i){
        for(int j = 0; j < img.size().width; ++j){
            Scalar intensity = img.at<uchar>(i,j);
            //std::cout << intensity.val[0] << std::endl;
        }
    }*/
    //namedWindow("image", CV_WINDOW_AUTOSIZE);


    return 0;
}
