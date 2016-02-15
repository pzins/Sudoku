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

public:
    GridRecognizer();
    GridRecognizer(const std::string& _filename);
    void setFilename(const std::string& _filename);
    Grid extractGrid();
    void printMat(cv::Mat& _m) const;
    void exportToFile(const std::string& _filename);

private:
    std::string imageFile;
    cv::Mat img;
    Grid grid;
    void removeGrid();
    int compute(int _caseHeight, int _caseWidth, int _x, int _y);
    int OCR(const std::string& _filename, cv::Mat& _img);

};




#endif
