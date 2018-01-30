#pragma once

#include"seg.h"
#include"predict.h"
#include"train.h"
#include <opencv.hpp>

int recognition(cv::Mat img);
long calc_area(Mat img);
int selectVaildArray(int(*array)[3], int length);
void sortArrayByY(int(*array)[3], int length);
inline void exchangeArray(int (*array)[3], int x1, int x2);
