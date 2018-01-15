#include "..\include\recognition.h"

int recognition(cv::Mat src)
{
	ANN_Wz _ann = ANN_Wz("param.xml");
	int result;
	int re[3][3] = { 0 };
	int length = 0;
	//Mat src = imread(fileName, 0);
	Mat img;
	preProcessing_wz(src, img, true);

	Mat flag;
	vector<cv::Point>seeds;
	seeds = connection_areas_4(img, flag);

	for (int i = 0; i < seeds.size(); i++)
	{
		cv::Point _seed = Point(seeds.back());
		int value = flag.at<uchar>(_seed.x,_seed.y);
		seeds.pop_back();
		Mat _temp;
		draw_areas(flag, _temp, value);
		long area = calc_area(_temp);
		if ((area > 2000) && (area < 50000)) {
			int _re = _ann.predict(_temp);
			if (_re != -1) {
				if (length > 2) {
					return -1;
				}
				re[length][0] = _seed.x;
				re[length][1] = _seed.y;
				re[length][2] = _re;
				length++;
			}
			/*cout << _re << endl;
			imshow("temp", _temp);
			cv::waitKey(0);
			destroyWindow("temp");*/
		}
		
	}

	if (2 == length) {
		if (re[0][1] > re[1][1]) {
			exchangeArray(re, 0, 1);
		}
		result = re[1][2] + re[0][2] * 10;
		if (((result % 5) != 0) || (0 == result)) {
			return -1;
		}
	}
	else if (1 == length) {
		result = re[0][2];
		if (result != 5) {
			return -1;
		}
	}
	else if (3 == length) {
		if (re[0][1] > re[1][1]) {
			exchangeArray(re, 0, 1);
		}
		if (re[0][1] > re[2][1]) {
			exchangeArray(re, 0, 2);
		}
		if (re[1][1] > re[2][1]) {
			exchangeArray(re, 1, 2);
		}

		result = re[0][2] * 100 + re[1][2] * 10 + re[2][2];
		if (((result % 5) != 0) || (0 == result)) {
			return -1;
		}
	}
	else
	{
		return -1;
	}
	
	return result;
}

long calc_area(Mat img)
{
	long count = 0;
	int rows = img.rows;
	int cols = img.cols;
	for (int i = 0; i < rows; i++)
	{
		uchar *ptr = img.ptr(i);
		for (int j = 0; j < cols; j++) {
			if (ptr[j] != 0) {
				count++;
			}
		}
	}

	return count;
}


void exchangeArray(int (*array)[3], int x1, int x2)
{
	int _temp;
	for (int i = 0; i < 3; i++) {
		_temp = array[x1][i];
		array[x1][i] = array[x2][i];
		array[x2][i] = _temp;
	}
}