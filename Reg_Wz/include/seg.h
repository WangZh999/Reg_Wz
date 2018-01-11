#pragma once
#include<opencv.hpp>
#include<vector>

#define uchar unsigned char

using namespace cv;
using namespace std;

/***************************************************************
//Ԥ����ͼ��������ָ����С���˲�����ֵ�ָ��̬ѧ����ʴ
//***************************************************************/
void preProcessing_wz(Mat src, Mat & dst, bool _show = false);

/***************************************************************
//��ͨ��Ѱ�ң��������ӵ㣻ʹ��8��ͨ��
//***************************************************************/
std::vector<cv::Point> connection_areas_8(Mat src, Mat & flag = Mat());

/***************************************************************
//��ͨ��Ѱ�ң��������ӵ㣻ʹ��4��ͨ��
//***************************************************************/
std::vector<cv::Point> connection_areas_4(Mat src, Mat & flag = Mat());

/***************************************************************
//��������
//***************************************************************/
unsigned long grow_Wz(Mat & img, Mat src, int seed_y, int seed_x);

void draw_areas(Mat flag, Mat & show_img, const uchar num);

bool value_replace(Mat &img, const uchar value_old, const uchar value_new, int rows = 0);

/***************************************************************
@param x1,x2,x3,x4 ����ֵ
@param sort �ź����ֵ
@return ������ͨ��ĸ���
//***************************************************************/
uchar select_area(const uchar* x, uchar * sort);