#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "sudoku.h"

using namespace cv;

Mat prewitt(Mat source)
{
    Mat img(source.size().height, source.size().width, CV_8UC1);
    int p00, p01, p02, p10, p12, p20, p21, p22;
    int H,V, GN;
    for(int i  = 0; i < img.size().height * img.size().width; ++i)
    {
        p00 = source.at<uchar>(i);
        p01 = source.at<uchar>(i+1);
        p02 = source.at<uchar>(i+2);
        p10 = source.at<uchar>(i+img.size().width);
        p12 = source.at<uchar>(i+img.size().width+2);
        p20 = source.at<uchar>(i+2*img.size().width);
        p21 = source.at<uchar>(i+2*img.size().width+1);
        p22 = source.at<uchar>(i+2*img.size().width+2);
        H = -p00-p01-p02+p20+p21+p22;
        V = -p00+p02-p10+p12-p20+p22;

        GN = abs(H) + abs(V);
        if(GN > 255) GN = 255;
        img.at<uchar>(i) = GN;
    }
    return img;
}


Mat gauss(Mat& source)
{
    Mat res(source.size().width, source.size().height, CV_8UC1);
    blur(source, res, Size(3,3));
    return res;
}
Mat sobel(Mat source)
{
    Mat img(source.size().height, source.size().width, CV_8UC1);
    int p00, p01, p02, p10, p12, p20, p21, p22;
    int H,V, GN;
    for(int i  = 0; i < img.size().height * img.size().width; ++i)
    {
        p00 = source.at<uchar>(i);
        p01 = source.at<uchar>(i+1);
        p02 = source.at<uchar>(i+2);
        p10 = source.at<uchar>(i+img.size().width);
        p12 = source.at<uchar>(i+img.size().width+2);
        p20 = source.at<uchar>(i+2*img.size().width);
        p21 = source.at<uchar>(i+2*img.size().width+1);
        p22 = source.at<uchar>(i+2*img.size().width+2);
        H = -p00-2*p01-p02+p20+2*p21+p22;
        V = -p00+p02-2*p10+2*p12-p20+p22;

        GN = abs(H) + abs(V);
        if(GN > 255) GN = 255;
        img.at<uchar>(i) = GN;
    }
    return img;
}


int main()
{
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
    };
    Sudoku s;
    s.import9x9FromFile();
    s.print();

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
    Mat img = imread("sudoku.png", CV_LOAD_IMAGE_GRAYSCALE);

    for(int i  = 0; i < img.size().height * img.size().width; ++i)
    {
        int p00;
        p00 = img.at<uchar>(i);
        std::cout << p00;
    }

    namedWindow("image", CV_WINDOW_AUTOSIZE);
    imshow("image", img);
    imshow("Sobel", sobel(img)); // Show frame
    imshow("gauss", gauss(img)); // Show frame
    imshow("prewitt", prewitt(img)); // Show frame

    waitKey();
    return 0;
}
