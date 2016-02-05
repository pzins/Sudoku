#include "gridRecognizer.h"
#include <fstream>


#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

//y => lines
//opencv : at : <rows, col>
//x => colmuns


int OCR(const std::string& _filename)
{
    char *outText;
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    api->Init(NULL, "fra");
    if (api->Init(NULL, "fra")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }

    api->SetVariable("tessedit_char_whitelist", "0123456789");

    // Open input image with leptonica library
    //api->SetPageSegMode(tesseract::PageSegMode::PSM_SINGLE_CHAR);
    Pix *image = pixRead(_filename.c_str());
    api->SetImage(image);
    // Get OCR result
    outText = api->GetUTF8Text();
    // Destroy used object and release memory
    api->End();
    std::string tmp = outText;
    //std::cout << "##  " << outText << std::endl;
    int ret = std::atoi(outText);
    delete outText;
    pixDestroy(&image);
    return ret;
}


GridRecognizer::GridRecognizer(const std::string& _filename, unsigned int _size) : imageFile(_filename),
    img(cv::imread(_filename, CV_LOAD_IMAGE_GRAYSCALE)), grid(_size, Line(_size))
{
}



void GridRecognizer::detectLine()
{
    for(int i = 0; i < img.size().width; ++i)
    {
        int isLine = true;
        double mean = 0;
        int successive = 0;
        for(int j = 0; j < img.size().height; ++j)
        {
            if(img.at<uchar>(j,i) < 100)
            {
                ++mean;
                ++successive;
            }
            else
            {
                successive = 0;
            }
            if(successive > img.size().height / 9)
            {
                successive = -1;
                break;
            }
        }
        if(mean / img.size().height > 0.5 || successive == -1)
        {
            for(int j = 0; j < img.size().height; ++j)
            {
                img.at<uchar>(j,i) = 255;
            }
        }

    }
    for(int j = 0; j < img.size().height; ++j)
    {
        int successive = 0;
        double mean = 0;
        for(int i = 0; i < img.size().width; ++i)
        {
            if(img.at<uchar>(j,i) < 100)
            {
                ++mean;
                ++successive;
            }
            else
            {
                successive = 0;
            }
            if(successive > img.size().width / 9)
            {
                successive = -1;
                break;
            }
        }
        if(mean / img.size().width > 0.5 || successive == -1)
        {
            for(int i = 0; i < img.size().width; ++i)
            {
                img.at<uchar>(j,i) = 255;
            }
        }

    }
//    cv::imshow("ol", img);
//    cv::imwrite("lyon.png", img);
//    cv::waitKey();
}

void GridRecognizer::color()
{
    for(int j = 0; j < img.size().height; ++j)
    {
        for(int i = 0; i < img.size().width; ++i)
        {
            if(img.at<uchar>(j,i) < 230)
            {
                img.at<uchar>(j,i) = 0;
            }
            else
            {
                img.at<uchar>(j,i) = 255;
            }

        }
    }
}


Grid GridRecognizer::getGrid()
{
    color();
//    cv::imshow("ololol", img);
//    cv::waitKey();
    detectLine();
    int size = grid.size();
    int caseHeight = img.size().height / size;
    int caseWidth = img.size().width / size;


    for(int y = 0; y < size; ++y)
    {
        for(int x = 0; x < size; ++x)
        {
            grid[y][x] = compute(caseHeight, caseWidth, x * caseWidth , y * caseHeight);
        }

    }
    return grid;
}








int GridRecognizer::compute(int _caseHeight, int _caseWidth, int _x, int _y)
{
    cv::Rect roi(_x, _y, _caseWidth, _caseHeight);
    cv::Mat out = img(roi);
    cv::Mat sudoku_th = out.clone();
    cv::adaptiveThreshold(out, sudoku_th, 255,
                                   CV_ADAPTIVE_THRESH_GAUSSIAN_C,
                                   CV_THRESH_BINARY_INV, 101, 1);
//    cv::imshow("out", sudoku_th);
//    cv::waitKey();
    cv::imwrite( "tmp.jpg", out);



    int ret = OCR("tmp.jpg");
//    std::cout << ">>>>>>>>>>>><  " << ret << std::endl;
    return ret;
}



//other tool functions

void GridRecognizer::exportToFile(const std::string &_filename)
{
    std::ofstream myfile;
    myfile.open (_filename);
    for(auto i : grid)
    {
        std::stringstream ss;
        for(auto j : i)
        {
            int a = j;
            ss << a;
            //ss << " ";
        }
        myfile << ss.str();
        myfile << '\n';
    }
    myfile.close();
}

void GridRecognizer::printMat(cv::Mat& _m) const
{
    for(int j = 0; j < _m.size().height; ++j)
    {
        for(int i = 0; i < _m.size().width; ++i)
        {
            cv::Scalar intensity = _m.at<uchar>(j,i);
            std::cout <<  intensity.val[0] << " ";
        }
        std::cout << std::endl;
    }
}
