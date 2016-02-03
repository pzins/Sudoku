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
    api->SetPageSegMode(tesseract::PageSegMode::PSM_SINGLE_CHAR);
    Pix *image = pixRead(_filename.c_str());
    api->SetImage(image);
    // Get OCR result
    outText = api->GetUTF8Text();
    // Destroy used object and release memory
    api->End();
    std::string tmp = outText;
    std::cout << "##  " << outText << std::endl;
    int ret = std::atoi(outText);
    delete outText;
    pixDestroy(&image);
    return ret;
}


GridRecognizer::GridRecognizer(const std::string& _filename, unsigned int _size) : imageFile(_filename),
    img(cv::imread(_filename, CV_LOAD_IMAGE_GRAYSCALE)), grid(_size, Line(_size))
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

cv::Mat thresh(cv::Mat img)
{
    cv::Mat s = img.clone();
    cv::adaptiveThreshold(img, s, 255,
                                   CV_ADAPTIVE_THRESH_GAUSSIAN_C,
                                   CV_THRESH_BINARY_INV, 101, 1);
    /*
    cv::Mat out = img.clone();
    cv::cvtColor(img,out,CV_BGR2GRAY);
    cv::Mat img_t=cv::Mat::zeros(img.size(),CV_8UC3);
    cv::adaptiveThreshold(out,img_t,255,cv::ADAPTIVE_THRESH_MEAN_C,cv::THRESH_BINARY_INV,5,10);*/
    return s;
}

cv::Mat grid_extract(cv::Mat img)
{
  int index;
  double max;
  cv::Mat grid;
  grid=cv::Mat::zeros(img.size(),CV_8UC1);
  std::vector<std::vector<cv::Point> > contour;
  std::vector<cv::Vec4i> h;
  std::vector<cv::Point> req;

  cv::findContours(img,contour,h,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE,cv::Point(0,0));
  max=cv::contourArea(contour[0]);
  for(int i=0;i<contour.size();i++)
  {
    double temp;
    temp=cv::contourArea(contour[i]);
    if(max<temp)
    {
      max=temp;
      index=i;
      req=contour[i];
    }
  }
  //cv::drawContours(grid,contour,index,cv::Scalar(255,255,255),CV_FILLED,8,h);
  cv::namedWindow("Grid",0);
  cv::imshow("Grid",grid);
  cv::waitKey(0);
  return grid(cv::boundingRect(req));
}

//Function to remove the lines from the grid(To seperate out digits from the grid)
cv::Mat hough(cv::Mat img)
{
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(img,lines,1,CV_PI/180,100,30,10);
    for(int i=0; i<lines.size();i++)
    {
        cv::Vec4i l=lines[i];
        cv::line(img,cv::Point(l[0],l[1]),cv::Point(l[2],l[3]),cv::Scalar(0,0,0),10,CV_AA);
    }
    cv::imshow("Digits",img);
    cv::waitKey(0);
    return img;
}


Grid GridRecognizer::getGrid()
{
    int size = grid.size();
    int caseHeight = img.size().height / size;
    int caseWidth = img.size().width / size;

    cv::imshow("ol", grid_extract(thresh(img)));
    cv::waitKey();
    //hough(grid_extract(thresh(img)));
    /*
    for(int y = 0; y < size; ++y)
    {
        for(int x = 0; x < size; ++x)
        {
            grid[y][x] = compute(caseHeight, caseWidth, x * caseWidth , y * caseHeight);
        }

    }*/
    return grid;
}








int GridRecognizer::compute(int _caseHeight, int _caseWidth, int _x, int _y)
{
    cv::Rect roi(_x+20, _y+20, _caseWidth-20, _caseHeight-20);
    cv::Mat out = img(roi);
    cv::Mat sudoku_th = out.clone();
    cv::adaptiveThreshold(out, sudoku_th, 255,
                                   CV_ADAPTIVE_THRESH_GAUSSIAN_C,
                                   CV_THRESH_BINARY_INV, 101, 1);
    cv::imshow("out", sudoku_th);
    cv::waitKey();
    cv::imwrite( "tmp.jpg", out);





    //cv::Mat in;
    //cv::resize(out, in, cv::Size(0,0), 10, 10);
    //cv::imwrite( std::to_string(_x) + std::to_string(_y) + ".jpg", in);
    //cv::imwrite( std::to_string(_x) + std::to_string(_y) + ".jpg", out);
    int ret = OCR("tmp.jpg");
    std::cout << ">>>>>>>>>>>><  " << ret << std::endl;
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
