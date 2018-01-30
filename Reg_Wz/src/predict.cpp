#include"..\include\define.h"
#include"..\include\predict.h"
#include"..\include\getFeature.h"
#include<string>
using namespace cv;

ANN_Wz::ANN_Wz()
{
	_Ann = ml::ANN_MLP::load("param.xml");//����ѵ���õ����������
	_feature_mat.convertTo(_feature_mat, CV_32F);	//���ڴ��ͼƬ��������
	_result = (Mat_<float>(1, CLASS_NUM));	//���ڴ��Ԥ����
											
	//_Ann1 = ml::ANN_MLP::load("param1.xml");//����ѵ���õ����������
	//_feature_mat1.convertTo(_feature_mat1, CV_32F);	//���ڴ��ͼƬ��������
	//_result1 = (Mat_<float>(1, CLASS_NUM));	//���ڴ��Ԥ����
}

ANN_Wz::ANN_Wz(string path)
{	
	_Ann = ml::ANN_MLP::load(path);//����ѵ���õ����������
	_feature_mat.convertTo(_feature_mat, CV_32F);	//���ڴ��ͼƬ��������
	_result = (Mat_<float>(1, CLASS_NUM));	//���ڴ��Ԥ����

	//_Ann1 = ml::ANN_MLP::load("param1.xml");//����ѵ���õ����������
	//_feature_mat1.convertTo(_feature_mat1, CV_32F);	//���ڴ��ͼƬ��������
	//_result1 = (Mat_<float>(1, CLASS_NUM));	//���ڴ��Ԥ����
}


int ANN_Wz::predict(cv::Mat img)
{
	float maxVal = -2;
	int result = 0;
	_feature_mat = getFeature(img);
	_Ann->predict(_feature_mat, _result);	//����������ȡ���
	for (int j = 0; j < CLASS_NUM; j++) {	//���������ԭ�� 
		float val = _result.at<float>(j);
		if (val > maxVal) {
			maxVal = val;
			result = j;
		}
	}


	//float maxVal1 = -2;
	//int result1 = 0;
	//_feature_mat1 = getFeature(img);
	//_Ann1->predict(_feature_mat1, _result1);	//����������ȡ���
	//for (int j = 0; j < CLASS_NUM; j++) {	//���������ԭ�� 
	//	float val = _result1.at<float>(j);
	//	if (val > maxVal1) {
	//		maxVal1 = val;
	//		result1 = j;
	//	}
	//}

	
	if ((maxVal < TH_MEMBERSHIP) && (maxVal < TH_MEMBERSHIP)) {
		return -1;
	}
	else {
		//if ((result1 == 4) && (maxVal < (maxVal1 + 0.4))) {
		/*if (result1 == 4) {
			return result1;
		}
		if ((result == 6) && (maxVal1 < (maxVal + 0.2))) {
			return result;
		}*/
		/*if (maxVal > maxVal1) {
			return result;
		}
		else {
			return result1;
		}*/

		return result;
	}
}


ANN_Wz::~ANN_Wz()
{
}