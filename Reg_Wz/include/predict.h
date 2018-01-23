#ifndef PREDICT_H
#define PREDICT_H

#include<opencv.hpp>

class ANN_Wz
{
public:
	ANN_Wz();
	ANN_Wz(std::string path);
	~ANN_Wz();

	int predict(cv::Mat img);

private:
	cv::Ptr<cv::ml::ANN_MLP> _Ann;
	cv::Mat _feature_mat;		//ͼ������������
	cv::Mat _result;

};

#endif