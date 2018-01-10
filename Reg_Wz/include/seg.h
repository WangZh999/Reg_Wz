#pragma once
#include<opencv.hpp>
#include<vector>

#define uchar unsigned char

using namespace cv;
using namespace std;

/***************************************************************
//预处理图像，缩放至指定大小、滤波、阈值分割、形态学处理腐蚀
//***************************************************************/
void preProcessing_wz(Mat src, Mat & dst);

/***************************************************************
//连通域寻找，返回种子点；使用8连通域；
//***************************************************************/
std::vector<cv::Point> connection_areas_8(Mat src, Mat & flag = Mat());

/***************************************************************
//连通域寻找，返回种子点；使用4连通域
//***************************************************************/
std::vector<cv::Point> connection_areas_4(Mat src, Mat & flag = Mat());

/***************************************************************
//区域生长
//***************************************************************/
unsigned long grow_Wz(Mat & img, Mat src, int seed_y, int seed_x);

void draw_areas(Mat flag, Mat & show_img, const uchar num);

bool value_replace(Mat &img, const uchar value_old, const uchar value_new, int rows = 0);

/***************************************************************
@param x1,x2,x3,x4 邻域值
@param sort 排好序的值
@return 相邻连通域的个数
//***************************************************************/
uchar select_area(const uchar* x, uchar * sort);