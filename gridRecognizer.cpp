#include "gridRecognizer.h"
#include <fstream>


GridRecognizer::GridRecognizer(const std::string& _filename) : imageFile(_filename),
    img(cv::imread(_filename, CV_LOAD_IMAGE_GRAYSCALE)), grid(9, Line(9))
{


}



Grid GridRecognizer::getGrid()
{
    int caseHeight = img.size().height / 9;
    int caseWidth = img.size().width / 9;


    for(int x = 0; x < 9; ++x)
    {
        for(int y = 0; y < 9; ++y)
        {
            grid.at(x).at(y) = compute(img, caseHeight, caseWidth, x * caseWidth, y * caseHeight);
        }
    }
    return grid;
}


int GridRecognizer::similarity(cv::Mat& _case, cv::Mat& _number)
{
    int somme = 0;
    for(int i = 0; i < _case.size().height; ++i){
        for(int j = 0; j < _case.size().width; ++j){
            cv::Scalar caseIntensity = _case.at<uchar>(i,j);
            cv::Scalar numberIntensity = _number.at<uchar>(i,j);
            if(std::abs(caseIntensity.val[0] - numberIntensity.val[0]) < 50)
            {
                ++somme;
            }
        }
    }
    return somme;
}


int GridRecognizer::compute(cv::Mat& _img, int _caseHeight, int _caseWidth, int _x, int _y)
{
    cv::Rect roi(2 + _x, 2 + _y, _caseHeight, _caseWidth);
    //Rect roi(2+_caseWidth*4, 2, _caseHeight, _caseWidth);
    cv::Mat crop = _img(roi);
    //imshow("base", crop);
    int max = 0, id = 0;
    for(int i = 0; i < 10; ++i)
    {
        std::string filename = "chiffres/" + std::to_string(i) + ".png";
        cv::Mat filtre = cv::imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
        cv::resize(filtre, filtre, crop.size());
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
