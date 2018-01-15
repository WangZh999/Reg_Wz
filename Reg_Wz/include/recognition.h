#pragma once

#include"seg.h"
#include"predict.h"
#include"train.h"
#include <opencv.hpp>

int recognition(cv::Mat img);
long calc_area(Mat img);
inline void exchangeArray(int (*array)[3], int x1, int x2);
