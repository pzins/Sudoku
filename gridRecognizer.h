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
    cv::Mat preCrop(cv::Mat _img); //const à add
    void printMat(cv::Mat& _m) const;


public:
    GridRecognizer(const std::string& _filename);

    Grid getGrid();
    int compute(int _caseHeight, int _caseWidth, int _x, int _y);

    int similarity(cv::Mat& _croppedCase, cv::Mat& _filter);
    cv::Mat calibrateFromOutline(cv::Mat& _img);
    cv::Mat calibrateFromCenter(cv::Mat& _img);//const static??
    void exportToFile(const std::string& _filename);










};





#endif
