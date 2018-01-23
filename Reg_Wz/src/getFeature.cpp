#include"..\include\define.h"
#include "..\include\getFeature.h"

Mat element = getStructuringElement(MORPH_RECT,
	Size(3, 3),
	Point(1, 1));

cv::Mat getFeature(Mat raw_img)
{	
	//ģ�����˲�
	blur(raw_img, raw_img, Size(3, 3));

	//��ֵ�ָ�
	threshold(raw_img, raw_img, 120, 255, CV_THRESH_OTSU);

	//Ѱ����С��Ӿأ��õ�����Ȥ����
	Rect _rect = getMinRect2(raw_img);
	Mat roi_img = raw_img(_rect);

	//��ͼƬ����ָֻ����С
	Mat lowData;
	resize(roi_img, lowData, Size(IMAGE_SIZE, IMAGE_SIZE));

	//ֱ��ͼ����
	Mat hist = getHistogram(lowData);

	resize(lowData, lowData, Size(LOWDATA_SIZE, LOWDATA_SIZE));

	Mat _feature(1, (2 * IMAGE_SIZE + LOWDATA_SIZE*LOWDATA_SIZE), CV_32F);

	for (int i = 0; i < (2 * IMAGE_SIZE); i++)
	{
		_feature.at<float>(i) = hist.at<float>(i);
	}

	
	for (int i = 0; i < LOWDATA_SIZE; i++)
	{
		unsigned char *ptr = lowData.ptr(i);
		for (int j = 0; j < LOWDATA_SIZE; j++)
		{
			_feature.at<float>(2 * IMAGE_SIZE + i * LOWDATA_SIZE + j) = (float)(ptr[j] / 255.0);
		}
	}
	return _feature;
}

cv::Rect getMinRect(Mat &img)
{
	unsigned short _top, _bottom, _left, _right;
	short cols, rows;
	cols = img.cols;
	rows = img.rows;
	_top = rows;
	_left = cols;
	_bottom = _right = 0;

	bool _flag = false;

	for (int i = 0; i < rows; i++)
	{
		unsigned char* ptr = img.ptr(i);		
		for (int j = 0; j < cols; j++)
		{
			if (ptr[j])
			{
				_flag = true;
				_left = j < _left ? j : _left;
				_right = j > _right ? j : _right;
			}
		}
		if (_flag)
		{
			_top = i < _top ? i : _top;
			_bottom = i > _bottom ? i : _bottom;
		}
	}
	if ((_right > _left)&(_bottom > _top))
	{
		return cv::Rect(_left, _top, _right - _left + 1, _bottom - _top + 1);
	}
	else
	{
		return cv::Rect(0, 0, cols, rows);
	}
}


cv::Rect getMinRect2(Mat &img)
{
	short cols, rows;
	unsigned short _top, _bottom, _left, _right;
	cols = img.cols;
	rows = img.rows;
	_bottom = rows;
	_right = cols;
	_top = _left = 0;

	float _value = 0.0f;

	Mat hist = getHistogram2(img);

	//�������ҵ�һ��������ֵ��������Ϊ��߽�
	for (int i = 0; i < cols; i++)
	{
		_value = hist.at<float>(i);
		if (_value < _THRESHOLD)	{
			//_left = _left > i ? _left : i;
			_left = i;
		}
		else {
			break;
		}
	}

	//���������һ��������ֵ��������Ϊ�ұ߽�
	for (int i = cols - 1; i >= 0; i--)
	{
		_value = hist.at<float>(i);
		if (_value < _THRESHOLD)	{
			//_right = _right < i ? _right : i;
			_right = i;
		}
		else {
			break;
		}
	}

	//�������µ�һ��������ֵ��������Ϊ�ϱ߽�
	for (int i = cols; i < rows + cols; i++)
	{
		_value = hist.at<float>(i);
		if (_value < _THRESHOLD)	{
			//_top = _top > (i - cols) ? _top : (i - cols);
			_top = i - cols;
		}
		else {
			break;
		}
	}

	//�������ϵ�һ��������ֵ��������Ϊ�±߽�
	for (int i = rows + cols - 1; i >= cols; i--)
	{
		_value = hist.at<float>(i);
		if (_value < _THRESHOLD)	{
			//_bottom = _bottom < (i - cols) ? _bottom : (i - cols);
			_bottom = i - cols;
		}
		else {
			break;
		}
	}



	if ((_right > _left)&(_bottom > _top))
	{
		return cv::Rect(_left, _top, _right - _left, _bottom - _top);
	}
	else
	{
		return cv::Rect(0, 0, cols, rows);
	}
}



/***************************
��ȡˮƽ����ֱ�����ֱ��ͼ
return��ˮƽ+��ֱ  ֱ��ͼ
**************************/
cv::Mat getHistogram(Mat &img)
{
	long hist_h[IMAGE_SIZE] = { 0 };
	long hist_v[IMAGE_SIZE] = { 0 };
	long total = 0;
	short cols, rows;
	cols = img.cols;
	rows = img.rows;
	for (int i = 0; i < rows; i++)
	{
		unsigned char* ptr = img.ptr(i);
		for (int j = 0; j < cols; j++)
		{
			hist_h[j] += ptr[j];
			hist_v[i] += ptr[j];
			total += ptr[j];
		}
	}

	//******************************************************************************
	float rate_count_h = 0.0;
	float rate_count_v = 0.0;
	float hist_h_f[IMAGE_SIZE] = { 0.0 };
	float hist_v_f[IMAGE_SIZE] = { 0.0 };
	for (int i = 0; i < IMAGE_SIZE; i++) {
		//rate_count_h += hist_h[i] / (float)total;
		//rate_count_v += hist_v[i] / (float)total;
		hist_h_f[i] = hist_h[i] / (((float)total) + 0.1);
		hist_v_f[i] = hist_v[i] / (((float)total) + 0.1);
	}

	Mat hist(1, 2 * IMAGE_SIZE, CV_32F);
	int hist_cols = hist.cols;
	for (int i = 0; i < IMAGE_SIZE; i++) {
		hist.at<float>(i) = hist_h_f[i];
	}
	for (int i = IMAGE_SIZE; i < hist_cols; i++) {
		hist.at<float>(i) = hist_h_f[i - IMAGE_SIZE];
	}
	//******************************************************************************/

	/******************************************************************************
	Mat hist_h_m(1, IMAGE_SIZE, CV_32FC1, hist_h);
	Mat hist_v_m(1, IMAGE_SIZE, CV_32FC1, hist_v);

	normalize(hist_h_m, hist_h_m, 1.0, 0.0, NORM_MINMAX);
	normalize(hist_v_m, hist_v_m, 1.0, 0.0, NORM_MINMAX);


	Mat hist(1, 2 * IMAGE_SIZE, CV_32F);
	int hist_cols = hist.cols;
	for (int i = 0; i < IMAGE_SIZE; i++)
	{
		hist.at<float>(i) = hist_h_m.at<float>(i); //(float)(hist_h[i]);
	}
	for (int i = IMAGE_SIZE; i < hist_cols; i++)
	{
		hist.at<float>(i) = hist_h_m.at<float>(i - IMAGE_SIZE);//(float)(hist_v[i - IMAGE_SIZE]);
	}
	//******************************************************************************/
	return hist;
}


/***************************
��ȡˮƽ����ֱ�����ֱ��ͼ(�ǹ̶���СͼƬ�����1000*1000��
return��ˮƽ+��ֱ  ֱ��ͼ
**************************/
cv::Mat getHistogram2(Mat &img)
{
	long hist_h[1000] = { 0 };
	long hist_v[1000] = { 0 };
	short cols, rows;
	cols = img.cols;
	rows = img.rows;

	if ((cols > 1000) || (rows > 1000))
	{
		return Mat();
	}

	for (int i = 0; i < rows; i++)
	{
		unsigned char* ptr = img.ptr(i);
		for (int j = 0; j < cols; j++)
		{
			hist_h[j] += ptr[j];
			hist_v[i] += ptr[j];
		}
	}
	Mat hist(1, cols + rows, CV_32F);
	int hist_cols = hist.cols;
	for (int i = 0; i < cols; i++)
	{
		hist.at<float>(i) = (float)(hist_h[i]) / cols;
	}
	for (int i = cols; i < rows + cols; i++)
	{
		hist.at<float>(i) = (float)(hist_v[i - cols]) / rows;
	}

	return hist;
}

