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
            grid.at(y).at(x) = compute(img, caseHeight, caseWidth, x * caseWidth, y * caseHeight);
        }
    }
    return grid;
}



int GridRecognizer::compute(cv::Mat& _img, int _caseHeight, int _caseWidth, int _x, int _y)
{
    //revoir crop je crois inversion width height
    cv::Rect roi(2 + _x, 2 + _y, _caseHeight-1, _caseWidth);
    //Rect roi(2+_caseWidth*4, 2, _caseHeight, _caseWidth);
    std::cout << "JUST BEFORE CROP roi" << std::endl;
    std::cout << 2 + _x << " / " << _y + 2 << std::endl;
    std::cout << roi.size().height << " / " << roi.size().width << std::endl;
    std::cout << _img.size().height << " / " << _img.size().width << std::endl;
    cv::Mat cropTemp = _img(roi);
    int caseHeight = img.size().height / 9;

    //if(_y == 2 * caseHeight)
      //  cv::imshow(std::to_string(_x) + "x" + std::to_string(_y), cropTemp);
    std::cout << _x << std::endl;
    std::cout << _y << std::endl;

    cv::Mat crop = calibrate(cropTemp);

    int max = 0, id = 0;
    for(int i = 0; i < 10; ++i)
    {
        std::string filename = "chiffres/" + std::to_string(i) + ".png";
        cv::Mat filtre = cv::imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
        std::cout << " # \t " << crop.size().width << "   " << crop.size().height << std::endl;
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
    std::cout << "END compute " << std::endl;
    return id;
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

cv::Mat calibrate(cv::Mat& _img)
{
    std::cout << "++++++++++++++++++++++++++" << std::endl;

    int top = 0;
    for(int i = 5; i < _img.size().height*0.5; ++i)
    {
        cv::Scalar intensity ;
        for(int j = 0; j < _img.size().width; ++j)
        {

            intensity = _img.at<uchar>(i,j);
            //std::cout << intensity.val[0] << " ";
            if(top == 0
                    && j < _img.size().width - _img.size().width * 0.2
                    && j > _img.size().width * 0.2
                    && intensity.val[0] < 100)
            {
                top = i;
                break;
            }
        }
        if(top != 0)
            break;
        //std::cout << std::endl;
    }
    std::cout << top << std::endl;

    int bottom = 0;
    for(int i = 5; i < _img.size().height*0.5; ++i)
    {
        cv::Scalar intensity ;
        for(int j = 0; j < _img.size().width; ++j)
        {

            intensity = _img.at<uchar>(_img.size().height - i - 1,j);
            if(bottom == 0
                    && j < _img.size().width - _img.size().width * 0.2
                    && j > _img.size().width * 0.2
                    && intensity.val[0] < 100)
            {
                bottom = _img.size().height- i - 1;
                break;
            }
        }
        if(bottom != 0)
            break;
    }
    std::cout << bottom << std::endl;

    int left = 0;
    for(int j = 5; j < _img.size().width*0.5; ++j)
    {
        cv::Scalar intensity ;
        for(int i = 0; i < _img.size().height; ++i)
        {

            intensity = _img.at<uchar>(i,j);
            if(left == 0
                    && i < _img.size().height - _img.size().height* 0.1
                    && i > _img.size().height * 0.1
                    && intensity.val[0] < 100)
            {
                left = j;
                break;
            }
        }
        if(left != 0)
            break;
    }
    std::cout << left << std::endl;

    int right = 0;
    for(int j = 5; j < _img.size().width*0.5; ++j)
    {
        cv::Scalar intensity ;
        for(int i = 0; i < _img.size().height; ++i)
        {

            intensity = _img.at<uchar>(i,_img.size().width - j - 1);
            if(right == 0
                    && i < _img.size().height - _img.size().height* 0.1
                    && i > _img.size().height * 0.1
                    && intensity.val[0] < 100)
            {
                right = _img.size().width - j -1;
                break;
            }
        }
        if(right != 0)
            break;
    }
    std::cout << right << std::endl;
    if(top == 0 && bottom == 0 && left == 0 && right == 0)
        return _img;
    std::cout << "-------------------------" << std::endl;
    cv::Rect roi(left, top, right - left + 1, bottom - top + 1);
    cv::Mat crop = _img(roi);
    return crop;
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
