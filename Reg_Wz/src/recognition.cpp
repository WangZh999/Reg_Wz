#include"..\include\define.h"
#include "..\include\recognition.h"

long count_ = 0;

int recognition(cv::Mat src)
{
	ANN_Wz _ann = ANN_Wz("param.xml");
	int result;
	int re[MAX_REG_NUM][3] = { 0 };
	int length = 0;
	//Mat src = imread(fileName, 0);
	Mat img;
	preProcessing_wz(src, img, true);
	//resize(src, img, Size(500, 500));


	Mat flag;
	vector<cv::Point>seeds;
	seeds = connection_areas_4(img, flag);

	int seeds_size = seeds.size();

	for (int i = 0; i < seeds_size; i++)
	{
		cv::Point _seed = Point(seeds.back());
		int value = flag.at<uchar>(_seed.x,_seed.y);
		seeds.pop_back();
		Mat _temp;
		draw_areas(flag, _temp, value);
		long area = calc_area(_temp);

		if (DEBUG_WZ) {
			cout << "area:  " << area << endl;
		}

		if ((area > 1500) && (area < 20000)) {

			int _re = _ann.predict(_temp);

			if (DEBUG_WZ) {
				cout << "re:  " << _re << endl << endl;
			}

			if (_re != -1) {
				if (length >= MAX_REG_NUM) {
					return -1;
				}
				re[length][0] = _seed.x;
				re[length][1] = _seed.y;
				re[length][2] = _re;
				length++;
			}
		}

		if (DEBUG_WZ) {
			imshow("temp", _temp);
			cv::waitKey(0);
			destroyWindow("temp");
		}		
	}

	if (2 == length) {
		if (re[0][1] > re[1][1]) {
			exchangeArray(re, 0, 1);
		}
		result = re[1][2] + re[0][2] * 10;
		if (((result % 5) != 0) || (0 == result)) {
			result = -1;
		}
	}
	else if (1 == length) {
		result = re[0][2];
		if (result != 5) {
			result = -1;
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
			result = -1;
		}
	}
	else
	{
		result = -1;
	}
	
	
	//string path_temp_ = "./out_temp/";
	//path_temp_.append(to_string(result)).append("_").append(to_string(count_)).append(".jpg");
	//try {
	//imwrite(path_temp_, img);
	//count_++;
	//}
	//catch (cv::Exception& ex) {
	//fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
	//return 1;
	//}


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


int selectVaildArray(int(*array)[3],int length)
{
	int dis[MAX_REG_NUM - 1] = { 0 };
	sortArrayByY(array, length);
	for (int i = 0; i < length - 1; i++) {
		dis[i] = array[i + 1][2] - array[i][2];
	}

	int min_pos = 0;
	for (int i = 1; i < length - 1; i++) {
		if (dis[min_pos] > dis[i]) {
			min_pos = i;
		}
	}

	if (0 == min_pos) {
		if ((dis[1] - dis[0]) < 20) {
			return 3;
		}
	}
	else if ((length - 2) == min_pos) {

	}
	else {

	}
	return 0;
}


void sortArrayByY(int(*array)[3], int length)
{
	for (int i = 0; i < length; i++)
	{
		for (int j = i + 1; j < length; j++)
		{
			if (array[i][2] > array[j][2])
			{
				exchangeArray(array, i, j);
			}
		}
	}
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