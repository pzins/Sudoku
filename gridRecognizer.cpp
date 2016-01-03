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
    // Open input image with leptonica library
    Pix *image = pixRead(_filename.c_str());
    api->SetImage(image);
    // Get OCR result
    outText = api->GetUTF8Text();
    // Destroy used object and release memory
    api->End();
    std::string tmp = outText;
    int ret = std::atoi(outText);
    delete [] outText;
    pixDestroy(&image);
    return ret;
}


GridRecognizer::GridRecognizer(const std::string& _filename) : imageFile(_filename),
    img(cv::imread(_filename, CV_LOAD_IMAGE_GRAYSCALE)), grid(9, Line(9))
{
}

Grid GridRecognizer::getGrid()
{
    int caseHeight = img.size().height / 9;
    int caseWidth = img.size().width / 9;

    for(int y = 0; y < 9; ++y)
    {
        for(int x = 0; x < 9; ++x)
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
    //cv::imshow("out", out);
    //cv::waitKey();
    cv::imwrite( "tmp.jpg", out);
    //cv::Mat in;
    //cv::resize(out, in, cv::Size(0,0), 10, 10);
    //cv::imwrite( std::to_string(_x) + std::to_string(_y) + ".jpg", in);
    //cv::imwrite( std::to_string(_x) + std::to_string(_y) + ".jpg", out);
    int ret = OCR("tmp.jpg") ;
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
