#include "gridRecognizer.h"
#include <fstream>

//opencv : at : <rows, line>
//x => colmuns
//y => lines


GridRecognizer::GridRecognizer(const std::string& _filename) : imageFile(_filename),
    img(cv::imread(_filename, CV_LOAD_IMAGE_GRAYSCALE)), grid(9, Line(9))
{


}



cv::Mat GridRecognizer::preCrop(cv::Mat _img)
{
    int x = _img.size().width * 0.15;
    int y = _img.size().height * 0.15;
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
            grid[y][x] = compute(caseHeight, caseWidth, x * caseWidth + (x/3), y * caseHeight+y/3);
        }

    }
    return grid;
}


//crop case from outline to have just the number without white border
cv::Mat GridRecognizer::calibrateFromOutline(cv::Mat& _img)
{
    //std::cout << "====================   START CALIBRATE 1   ===================" << std::endl;
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
    //std::cout << "====================   END CALIBRATE 1  ===================" << std::endl;
    return crop;
}


//crop case from center point to have just the number without white border
cv::Mat GridRecognizer::calibrateFromCenter(cv::Mat& _img)
{
    //std::cout << "====================   START CALIBRATE 2   ===================" << std::endl;

    //define start/end index to eliminate black outline problem
    float coeff = 0;
    int beginX = _img.size().width * coeff;
    int endX = _img.size().width - beginX;
    int beginY = _img.size().height * coeff;
    int endY = _img.size ().height - beginY;


    int centerX = _img.size().width / 2;
    int centerY = _img.size().height / 2;

    cv::Scalar intensity;
    int top = centerY;
    for(int j = centerY; j >= 0; --j)
    {
        bool isVoid = true;
        for(int i = beginX; i < endX; ++i)
        {
            intensity = _img.at<uchar>(j, i);
            if(intensity.val[0] < 50)
            {
                isVoid = false;
            }
        }
        if(isVoid){
            top = j;
            break;
        }
    }
    //std::cout << "top = " << top << std::endl;

    int bottom = centerY;
    for(int j = centerY; j < _img.size().height; ++j)
    {
        bool isVoid = true;
        for(int i = beginX; i < endX; ++i)
        {
            intensity = _img.at<uchar>(j, i);
            if(intensity.val[0] < 50)
            {
                isVoid = false;
            }
        }
        if(isVoid){
            bottom = j;
            break;
        }
    }
    //std::cout << "bottom = " << bottom << std::endl;

    int left = centerX;
    for(int i = centerX; i >= 0; --i)
    {
        bool isVoid = true;
        for(int j = beginY; j < endY; ++j)
        {
            intensity = _img.at<uchar>(j, i);
            if(intensity.val[0] < 50)
            {
                isVoid = false;
            }
        }
        if(isVoid){
            left = i;
            break;
        }
    }
    //std::cout << "left = " << left << std::endl;

    int right = centerX;
    for(int i = centerX; i < _img.size().width; ++i)
    {
        bool isVoid = true;
        for(int j = beginY; j < endY; ++j)
        {
            intensity = _img.at<uchar>(j, i);
            if(intensity.val[0] < 50)
            {
                isVoid = false;
            }
        }
        if(isVoid){
            right = i;
            break;
        }
    }
    //std::cout << "right = " << right << std::endl;

    if(top == centerY && bottom == centerY && left == centerX && right == centerX)
        return _img;
    cv::Rect roi(left, top, right - left+1, bottom - top+1);
    cv::Mat crop = _img(roi);
    //std::cout << "====================   END CALIBRATE 2   ===================" << std::endl;
    return crop;

}


int GridRecognizer::compute(int _caseHeight, int _caseWidth, int _x, int _y)
{
    cv::Rect roi(_x, _y, _caseWidth, _caseHeight);
    cv::Mat singleCase = preCrop(img(roi));
    cv::Mat croppedCase = calibrateFromCenter(singleCase);
    cv::Scalar intensity;
    //cv::imshow(std::to_string(_x) + std::to_string(_y), singleCase);
    //cv::imshow(std::to_string(_x) + std::to_string(_y)+"_cropped", croppedCase);
    //cv::waitKey()

    int max = 0, id = 0;
    std::cout << "======  " << _x/_caseWidth << "    " << _y/_caseHeight << "==========" << std::endl;
    //if(_x/_caseWidth == 8 && _y/_caseHeight == 4)
    //    cv::imshow("ol", croppedCase);
    for(int i = 0; i < 10; ++i)
    {
        int tmp = 0;
        for(int j = 0; j < 4; ++j)
        {
            std::string filename = "chiffres/" + std::to_string(i) + "_" + std::to_string(j) + ".png";
            cv::Mat filter = cv::imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
            cv::resize(filter, filter, croppedCase.size(), cv::INTER_LANCZOS4);//others options possible for resize see doc
            tmp += similarity(croppedCase, filter);
        }
        if(tmp > max)
        {
            max = tmp;
            id = i;
        }
        std::cout << i << " => " << tmp << std::endl;
    }
    std::cout << "============END=============" << std::endl;
    //cv::waitKey();
    //cv::destroyWindow(std::to_string(_x) + std::to_string(_y)+"_cropped");
    return id;

}


int GridRecognizer::similarity(cv::Mat& _croppedCase, cv::Mat& _filter)
{
    //cv::imshow("filtre", _filter);
    //cv::imshow("case", _croppedCase);
    //cv::waitKey();
    int somme = 0;
    for(int j = 0; j < _croppedCase.size().height; ++j){
        for(int i = 0; i < _croppedCase.size().width; ++i){
            cv::Scalar caseIntensity = _croppedCase.at<uchar>(j,i);
            cv::Scalar numberIntensity = _filter.at<uchar>(j,i);
            if(std::abs(caseIntensity.val[0] - numberIntensity.val[0]) < 5 && caseIntensity.val[0] < 10
                    && numberIntensity.val[0] < 10)
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
