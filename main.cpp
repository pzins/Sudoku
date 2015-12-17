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



int similarity(Mat& _case, Mat& _number)
{
    int somme = 0;
    for(int i = 0; i < _case.size().height; ++i){
        for(int j = 0; j < _case.size().width; ++j){
            Scalar caseIntensity = _case.at<uchar>(i,j);
            Scalar numberIntensity = _number.at<uchar>(i,j);
            if(std::abs(caseIntensity.val[0] - numberIntensity.val[0]) < 50)
            {
                ++somme;
            }
        }
    }
    return somme;
}


int compute(Mat& _img, int _caseHeight, int _caseWidth, int _x, int _y)
{
    Rect roi(2 + _x, 2 + _y, _caseHeight, _caseWidth);
    //Rect roi(2+_caseWidth*4, 2, _caseHeight, _caseWidth);
    Mat crop = _img(roi);
    //imshow("base", crop);
    int max = 0, id = 0;
    for(int i = 0; i <= 10; ++i)
    {
        std::string filename = "chiffres/" + std::to_string(i) + ".png";
        Mat filtre = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
        resize(filtre, filtre, crop.size());
        //imshow(std::to_string(i), filtre);
        int tmp = similarity(crop, filtre);
        //std::cout << i << " => " << tmp << std::endl;
        if(tmp > max)
        {
            max = tmp;
            id = i;
        }
    }
    return id;
}

int main()
{


    Mat img = imread("sudoku.png", CV_LOAD_IMAGE_GRAYSCALE);
    int caseHeight = img.size().height / 9;
    int caseWidth = img.size().width / 9;

    int mat[9][9];

    for(int x = 0; x < 9; ++x)
    {
        for(int y = 0; y < 9; ++y)
        {
            mat[x][y] = compute(img, caseHeight, caseWidth, x * caseWidth, y * caseHeight);
        }
    }


    for(int i = 0; i < 9; ++i)
    {
        for(int j = 0; j < 9; ++j)
        {
            std::cout << mat[j][i] << " ";
        }
        std::cout << std::endl;
    }
    //imshow("crop", crop);

    /*
    imshow("un", img);

    Mat im(Size(100,100), 0);

    resize(img, im, im.size());
*/

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
