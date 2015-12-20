#include "gridRecognizer.h"
#include <fstream>


GridRecognizer::GridRecognizer(const std::string& _filename) : imageFile(_filename),
    img(cv::imread(_filename, CV_LOAD_IMAGE_GRAYSCALE)), grid(9, Line(9))
{


}

cv::Mat preCrop(cv::Mat& _img)
{
    int x = _img.size().width * 0.2;
    int y = _img.size().height * 0.2;
    cv::Rect roi(x, y, _img.size().width - 2 * x, _img.size().height - 2 * y);
    cv::Mat ret = _img(roi);
    return ret;
}

Grid GridRecognizer::getGrid()
{
    int caseHeight = img.size().height / 9;
    int caseWidth = img.size().width / 9;


    for(int y = 0; y < 9; ++y)
    {
        for(int x = 0; x < 9; ++x)
        {
            grid[y][x] = compute(img, caseHeight, caseWidth, x * caseWidth, y * caseHeight);
        }

    }
    return grid;
}


cv::Mat calibrate(cv::Mat& _img)
{
    //std::cout << "====================   START CALIBRATE   ===================" << std::endl;
    int top = 0;
    cv::Scalar intensity ;
    for(int j = 0; j < _img.size().height; ++j)
    {
        for(int i = 0; i < _img.size().width; ++i)
        {

            intensity = _img.at<uchar>(j,i);
            if(top == 0 && intensity.val[0] < 100)
            {
                top = j;
                break;
            }
        }
        if(top != 0)
            break;
    }
    //std::cout << "top = " << top << std::endl;

    int bottom = 0;
    for(int j = _img.size().height - 1; j >= 0; --j)
    {
        for(int i = 0; i < _img.size().width; ++i)
        {

            intensity = _img.at<uchar>(j, i);
            if(bottom == 0 && intensity.val[0] < 100)
            {
                bottom = j;
                break;
            }
        }
        if(bottom != 0)
            break;
    }
    //std::cout << "bottom = " << bottom << std::endl;

    int left = 0;
    for(int i = 0; i < _img.size().width; ++i)
    {
        for(int j = 0; j < _img.size().height; ++j)
        {
            intensity = _img.at<uchar>(j, i);
            if(left == 0 && intensity.val[0] < 100)
            {
                left = i;
                break;
            }
        }
        if(left != 0)
            break;
    }
    //std::cout << "left = " << left << std::endl;

    int right = 0;
    for(int i = _img.size().width - 1; i >= 0; --i)
    {
        for(int j = 0; j < _img.size().height; ++j)
        {

            intensity = _img.at<uchar>(j, i);
            if(right == 0 && intensity.val[0] < 100)
            {
                right = i;
                break;
            }
        }
        if(right != 0)
            break;
    }
   // std::cout << "right = " << right << std::endl;
    if(top == 0 && bottom == 0 && left == 0 && right == 0)
        return _img;
    cv::Rect roi(left, top, right - left+1, bottom - top+1);
    cv::Mat crop = _img(roi);
    //std::cout << "====================   END CALIBRATE   ===================" << std::endl;
    return crop;
}


//opencv : at : <rows, line>
//x => colmuns
//y => lines

int GridRecognizer::compute(cv::Mat& _img, int _caseHeight, int _caseWidth, int _x, int _y)
{
    cv::Rect roi(_x, _y, _caseWidth, _caseHeight);
    cv::Mat cropTemp = _img(roi);


    cv::Mat tmp = preCrop(cropTemp);
    cv::Mat result = calibrate(tmp);
    cv::Scalar intensity ;

    /*
    if(_y == 6 * _caseHeight)
    {
        std::cout << _x << " / " << _y << std::endl;
        cv::imshow(std::to_string(_x) + "a", cropTemp);
        cv::imshow(std::to_string(_x) + "b", tmp);
        cv::imshow(std::to_string(_x), result);

    }
    */

    /*
    std::cout << "==========================================" << std::endl;
    for(int j = 0; j < result.size().height; ++j)
    {
        for(int i = 0; i < result.size().width; ++i)
        {
            intensity = result.at<uchar>(j,i);
            std::cout << intensity.val[0] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "==========================================" << std::endl;
    */
    //cv::waitKey(0);

    cv::Mat crop = result;

    int max = 0, id = 0;
    for(int i = 0; i < 10; ++i)
    {
        std::string filename = "chiffres/" + std::to_string(i) + ".png";
        cv::Mat filtre = cv::imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
        cv::resize(filtre, filtre, crop.size());
        int tmp = similarity(crop, filtre);
        if(tmp > max)
        {
            max = tmp;
            id = i;
        }
    }
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
                ++somme;
        }
    }
    return somme;
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
