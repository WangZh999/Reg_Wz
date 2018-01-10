#ifndef PREDICT_H
#define PREDICT_H

#include<opencv.hpp>

#define CLASS_NUM 10
#define TH_MEMBERSHIP 0.5

class ANN_Wz
{
public:
	ANN_Wz();
	ANN_Wz(std::string path);
	~ANN_Wz();

	int predict(cv::Mat img);

private:
	cv::Ptr<cv::ml::ANN_MLP> _Ann;
	cv::Mat _feature_mat;		//图像特征向量集
	cv::Mat _result;

};

#endif