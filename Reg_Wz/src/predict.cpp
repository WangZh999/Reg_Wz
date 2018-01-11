#include"..\include\predict.h"
#include"..\include\getFeature.h"
#include<string>
using namespace cv;

ANN_Wz::ANN_Wz()
{
	ANN_Wz("../../OCR/param.xml");
}

ANN_Wz::ANN_Wz(string path)
{
	/*载入训练好的神经网络参数*/
	_Ann = ml::ANN_MLP::load(path);
	_feature_mat.convertTo(_feature_mat, CV_32F);	//用于存放图片特征向量
	_result = (Mat_<float>(1, CLASS_NUM));	//用于存放预测结果
}


int ANN_Wz::predict(cv::Mat img)
{
	float maxVal = -2;
	int result = 0;
	_feature_mat = getFeature(img);
	_Ann->predict(_feature_mat, _result);	//根据特征求取结果
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               	for (int j = 0; j < CLASS_NUM; j++) {	//最大隶属度原则
		float val = _result.at<float>(j);
		if (val > maxVal) {
			maxVal = val;
			result = j;
		}
	}
	
	if (maxVal < TH_MEMBERSHIP)
	{
		return -1;
	}
	else
	{
		return result;
	}
}


ANN_Wz::~ANN_Wz()
{
}