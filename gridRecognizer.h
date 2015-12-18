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
    GridRecognizer(const std::string& _filename);

    Grid getGrid();
    int compute(cv::Mat& _img, int _caseHeight, int _caseWidth, int _x, int _y);

    int similarity(cv::Mat& _case, cv::Mat& _number);

    void exportToFile(const std::string& _filename);










};





#endif
