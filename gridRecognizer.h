#ifndef GRIDRECOGNIZER_H
#define GRIDRECOGNIZER_H
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "sudokuSolver.h"
#include <vector>


using Grid = std::vector<std::vector<int>>;
using Line = std::vector<int>;

class GridRecognizer
{
private:
    const std::string imageFile;
    cv::Mat img;
    Grid grid;




public:
    GridRecognizer(const std::string& _filename, unsigned int _size);

    void printMat(cv::Mat& _m) const;
    Grid getGrid();
    int compute(int _caseHeight, int _caseWidth, int _x, int _y);


    void exportToFile(const std::string& _filename);


};




#endif
