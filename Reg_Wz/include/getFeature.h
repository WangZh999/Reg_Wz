#pragma once
#ifndef  GETFEATURE_H
#define	 GETFEATURE_H


#include<opencv.hpp>

using namespace cv;
using namespace std;


cv::Mat getFeature(Mat img);
cv::Rect getMinRect(Mat &img);
cv::Rect getMinRect2(Mat &img);
cv::Mat getHistogram(Mat &img);
cv::Mat getHistogram2(Mat &img);

#endif // ! GETFEATURE_H