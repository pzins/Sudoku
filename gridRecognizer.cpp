#include "gridRecognizer.h"
#include <fstream>


#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

//y => lines
//opencv : at : <rows, col>
//x => colmuns
#define THRESHOLDCOLOR 230

int OCR(const std::string& _filename)
{
    char *outText;
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    if (api->Init(NULL, "fra")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }
    //api->SetVariable("tessedit_char_whitelist", "0123456789");
    //api->SetPageSegMode(tesseract::PageSegMode::PSM_SINGLE_CHAR);

    // Open input image with leptonica library
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


GridRecognizer::GridRecognizer(const std::string& _filename) : imageFile(_filename),
    img(cv::imread(_filename, CV_LOAD_IMAGE_GRAYSCALE)), grid(9, Line(9))
{
}

GridRecognizer::GridRecognizer() : grid(9, Line(9))
{
}

void GridRecognizer::setFilename(const std::string& _filename)
{
    imageFile = _filename;
}




void GridRecognizer::removeGrid()
{
    for(int i = 0; i < img.size().width; ++i)
    {
        int isLine = true;
        double mean = 0;
        int successive = 0;
        for(int j = 0; j < img.size().height; ++j)
        {
            if(img.at<uchar>(i, j) < THRESHOLDCOLOR)
            {
                img.at<uchar>(i, j) = 0;
                ++mean;
                ++successive;
            }
            else
            {
                img.at<uchar>(i, j) = 255;
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
}

void GridRecognizer::thresholdColor()
{
    for(int j = 0; j < img.size().height; ++j)
    {
        for(int i = 0; i < img.size().width; ++i)
        {
            if(img.at<uchar>(j,i) < THRESHOLDCOLOR)
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


Grid GridRecognizer::extractGrid()
{
    img = cv::imread(imageFile, CV_LOAD_IMAGE_GRAYSCALE);
    //thresholdColor();
    removeGrid();
    cv::imshow("ol", grid);
    cv::waitKey();
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
    cv::imwrite( "tmp.jpg", out);
    int ret = OCR("tmp.jpg");
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
