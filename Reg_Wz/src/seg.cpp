#include"..\include\seg.h"

void preProcessing_wz(Mat src, Mat & dst, bool _show)
{
	resize(src, src, Size(500, 500));
	Mat img(src.size(), CV_8UC1);
	//bilateralFilter(src, img, 5, 100, 100);
	blur(src, img, Size(5, 5));
	medianBlur(src, img, 5);

	if (_show) {
		imshow("src", img);
	}

	threshold(img, img, 0, 255, THRESH_OTSU);
	threshold(img, dst, 70, 255, THRESH_BINARY_INV);
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 13));
	erode(dst, dst, element);
}


std::vector<cv::Point> connection_areas_8(Mat src, Mat & flag)
{
	int rows = src.rows;
	int cols = src.cols;

	short area_count = 0;

	uchar area_x[4] = { 0 };
	uchar area_sort[4];
	int area_nums;

	std::vector<cv::Point> seeds;
	//初始化标记图
	flag = Mat(rows, cols, CV_8UC1);
	for (int i = 0; i < rows; i++)
	{
		uchar *ptr = flag.ptr(i);
		for (int j = 0; j < cols; j++)
		{
			ptr[j] = 0;
		}
	}

	//对原图进行阈值分割，如果已经是二值图像(0,255)则不受影响
	threshold(src, src, 100, 255, THRESH_BINARY);

	//连通域，去除最上一行，第一列和最后一列;
	for (int i = 1; i < rows; i++)
	{
		uchar *ptr_src = src.ptr(i);
		uchar *ptr_flag_1 = flag.ptr(i - 1);
		uchar *ptr_flag_0 = flag.ptr(i);
		for (int j = 1; j < cols - 1; j++)
		{
			if (255 == ptr_src[j])
			{
				area_x[0] = ptr_flag_1[j - 1];
				area_x[1] = ptr_flag_1[j];
				area_x[2] = ptr_flag_1[j + 1];
				area_x[3] = ptr_flag_0[j - 1];
				area_nums = select_area(area_x, area_sort);
				if (0 == area_nums) {
					area_count++;
					if(area_count>255)
					{
						return seeds;
					}
					ptr_flag_0[j] = (uchar)area_count;
				}
				else
				{
					uchar area_bleong = area_sort[4 - area_nums];
					ptr_flag_0[j] = area_bleong;
					for (int k = (5 - area_nums); k < 4; k++) {
						value_replace(flag, area_sort[i], area_bleong);
					}
				}
			}
		}
	}


	Mat _temp_flag = flag.clone();
	for (int i = 0; i < rows; i++)
	{
		uchar *ptr = _temp_flag.ptr(i);
		for (int j = 0; j < cols; j++)
		{
			if (ptr[j] != 0)
			{
				seeds.push_back(Point(i, j));
				value_replace(_temp_flag, ptr[j], 0);
			}
		}
	}

	 return seeds;
}

std::vector<cv::Point> connection_areas_4(Mat src, Mat & flag)
{
	bool areas_table[256] = { false };

	int rows = src.rows;
	int cols = src.cols;

	short area_count = 0;

	int area_num;
	
	std::vector<cv::Point> seeds;
	//初始化标记图
	flag = Mat(rows, cols, CV_8UC1);
	for (int i = 0; i < rows; i++)
	{
		uchar *ptr = flag.ptr(i);
		for (int j = 0; j < cols; j++)
		{
			ptr[j] = 0;
		}
	}

	//对原图进行阈值分割，如果已经是二值图像(0,255)则不受影响
	threshold(src, src, 100, 255, THRESH_BINARY);

	//连通域，去除最上一行，第一列和最后一列;
	for (int i = 1; i < rows; i++)
	{
		uchar *ptr_src = src.ptr(i);
		uchar *ptr_flag_1 = flag.ptr(i - 1);
		uchar *ptr_flag_0 = flag.ptr(i);
		for (int j = 1; j < cols - 1; j++)
		{
			if (255 == ptr_src[j])
			{
				if ((0 != ptr_flag_0[j - 1]) && (0 != ptr_flag_1[j]))
				{
					if (ptr_flag_0[j - 1] > ptr_flag_1[j]) {
						ptr_flag_0[j] = ptr_flag_1[j];
						areas_table[ptr_flag_0[j - 1]] = false;
						value_replace(flag, ptr_flag_0[j - 1], ptr_flag_1[j], i + 1);
						area_count--;
					}
					else if (ptr_flag_0[j - 1] < ptr_flag_1[j]) {
						ptr_flag_0[j] = ptr_flag_0[j - 1];
						areas_table[ptr_flag_1[j]] = false;
						value_replace(flag, ptr_flag_1[j], ptr_flag_0[j - 1], i + 1);
						area_count--;
					}
					else {
						ptr_flag_0[j] = ptr_flag_1[j];
					}
				}
				else if (ptr_flag_0[j - 1] != 0) {
					ptr_flag_0[j] = ptr_flag_0[j - 1];
				}
				else if (0 != ptr_flag_1[j]) {
					ptr_flag_0[j] = ptr_flag_1[j];
				}
				else {
					for (int _i = 1; _i < 256; _i++) {
						if (!areas_table[_i]) {
							area_num = _i;
							break;
						}
					}
					area_count++;
					if (area_count > 255) {
						return seeds;
					}
					ptr_flag_0[j] = area_num;
					areas_table[area_num] = true;
				}

			}//end if (255 == ptr_src[j])
		}
	}


	Mat _temp_flag = flag.clone();
	for (int i = 0; i < rows; i++)
	{
		uchar *ptr = _temp_flag.ptr(i);
		for (int j = 0; j < cols; j++)
		{
			if (ptr[j] != 0)
			{
				seeds.push_back(Point(i, j));
				value_replace(_temp_flag, ptr[j], 0);
			}
		}
	}

	return seeds;
}

void draw_areas(Mat flag, Mat & show_img, const uchar num)
{
	int rows = flag.rows;
	int cols = flag.cols;
	show_img = Mat(flag.size(), CV_8UC1);
	for (int i = 0; i < rows; i++)
	{
		uchar* ptr_f = flag.ptr(i);
		uchar *ptr_s = show_img.ptr(i);
		for (int j = 0; j < cols; j++)
		{
			if (num == ptr_f[j]) {
				ptr_s[j] = 255;
			}
			else {
				ptr_s[j] = 0;
			}
		}
	}
}

unsigned long grow_Wz(Mat & img, Mat src, int seed_y, int seed_x)
{
	unsigned long length = 0;
	bool _up, _down, _left, _right;
	_up = _down = _left = _right = false;

	//*********************************
	//八邻域
	bool _up_right, _up_left, _down_right, _down_left;
	_up_right = _up_left = _down_right = _down_left = false;
	//*********************************

	if ((seed_y + 1) < img.rows) {
		if ((255 == src.at<unsigned char>(seed_y + 1, seed_x)) \
			&& (0 == img.at<unsigned char>(seed_y + 1, seed_x)))
		{
			img.at<unsigned char>(seed_y + 1, seed_x) = 255;
			length++;
			_up = true;
			//length += grow_Wz(img, src, seed_y + 1, seed_x);
		}
	}

	if ((seed_y - 1) >= 0) {
		if ((255 == src.at<unsigned char>(seed_y - 1, seed_x)) \
			&& (0 == img.at<unsigned char>(seed_y - 1, seed_x)))
		{
			img.at<unsigned char>(seed_y - 1, seed_x) = 255;
			length++;
			_down = false;
			//length += grow_Wz(img, src, seed_y - 1, seed_x);
		}
	}

	if ((seed_x + 1) < img.cols) {
		if ((255 == src.at<unsigned char>(seed_y, seed_x + 1)) \
			&& (0 == img.at<unsigned char>(seed_y, seed_x + 1)))
		{
			img.at<unsigned char>(seed_y, seed_x + 1) = 255;
			length++;
			_right = true;
			//length += grow_Wz(img, src, seed_y, seed_x + 1);
		}
	}

	if ((seed_x - 1) >= 0) {
		if ((255 == src.at<unsigned char>(seed_y, seed_x - 1)) \
			&& (0 == img.at<unsigned char>(seed_y, seed_x - 1)))
		{
			img.at<unsigned char>(seed_y, seed_x - 1) = 255;
			length++;
			_left = true;
			//length += grow_Wz(img, src, seed_y, seed_x - 1);
		}
	}


	//*****************************************
	//八邻域
	if (((seed_y + 1) < img.rows) && (((seed_x + 1) < img.cols))) {
		if ((255 == src.at<unsigned char>(seed_y + 1, seed_x + 1)) \
			&& (0 == img.at<unsigned char>(seed_y + 1, seed_x + 1)))
		{
			img.at<unsigned char>(seed_y + 1, seed_x + 1) = 255;
			length++;
			_up_right = true;
			//length += grow_Wz(img, src, seed_y + 1, seed_x + 1);
		}
	}

	if (((seed_y + 1) < img.rows) && (((seed_x - 1) >= 0))) {
		if ((255 == src.at<unsigned char>(seed_y + 1, seed_x - 1)) \
			&& (0 == img.at<unsigned char>(seed_y + 1, seed_x - 1)))
		{
			img.at<unsigned char>(seed_y + 1, seed_x - 1) = 255;
			length++;
			_up_left = true;
			//length += grow_Wz(img, src, seed_y + 1, seed_x - 1);
		}
	}

	if (((seed_y - 1) >= 0) && (((seed_x + 1) < img.cols))) {
		if ((255 == src.at<unsigned char>(seed_y - 1, seed_x + 1)) \
			&& (0 == img.at<unsigned char>(seed_y - 1, seed_x + 1)))
		{
			img.at<unsigned char>(seed_y - 1, seed_x + 1) = 255;
			length++;
			_down_right = true;
			//length += grow_Wz(img, src, seed_y - 1, seed_x + 1);
		}
	}

	if (((seed_y - 1) >= 0) && (((seed_x - 1) >= 0))) {
		if ((255 == src.at<unsigned char>(seed_y - 1, seed_x - 1)) \
			&& (0 == img.at<unsigned char>(seed_y - 1, seed_x - 1)))
		{
			img.at<unsigned char>(seed_y - 1, seed_x - 1) = 255;
			length++;
			_down_left = true;
			//length += grow_Wz(img, src, seed_y - 1, seed_x - 1);
		}
	}
	//******************************/




	if (_up) {
		length += grow_Wz(img, src, seed_y + 1, seed_x);
	}
	if (_down) {
		length += grow_Wz(img, src, seed_y - 1, seed_x);
	}
	if (_right) {
		length += grow_Wz(img, src, seed_y, seed_x + 1);
	}
	if (_left) {
		length += grow_Wz(img, src, seed_y, seed_x - 1);
	}



	//*****************************************
	//八邻域
	if (_up_right) {
		length += grow_Wz(img, src, seed_y + 1, seed_x + 1);
	}
	if (_up_left) {
		length += grow_Wz(img, src, seed_y + 1, seed_x - 1);
	}
	if (_down_right) {
		length += grow_Wz(img, src, seed_y - 1, seed_x + 1);
	}
	if (_down_left) {
		length += grow_Wz(img, src, seed_y - 1, seed_x - 1);
	}
	//******************************/

	return length;
}

bool value_replace(Mat &img, const uchar value_old, const uchar value_new, int rows)
{
	if (0 == rows) {
		rows = img.rows;
	}
	if (value_old == value_new) {
		return true;
	}
	for (int i = 0; i < rows; i++)
	{
		uchar* ptr = img.ptr(i);
		for (int j = 0; j < img.cols; j++)
		{
			if (value_old == ptr[j])
			{
				ptr[j] = value_new;
			}
		}
	}
	return true;
}

uchar select_area(const uchar * x, uchar * sort)
{
	if ((x[0] == 0) && (x[1] == 0) \
		&& (x[2] == 0) && (x[3] == 0))
	{
		return 0;
	}
	else
	{
		int _num = 0;
		//number of 0
		for (int i = 0; i < 4; i++)	{
			if (x[i] == 0){
				_num++;
			}
		}

		for (int i = 0; i < 4; i++) {
			sort[i] = x[i];
		}

		for (int i = 0; i < 4; i++) {
			for (int j = i + 1; j < 4; j++)
			{
				if (sort[j] < sort[i]) {
					uchar _temp = sort[j];
					sort[j] = sort[i];
					sort[i] = _temp;
				}

			}
		}

		return (4 - _num);
	}
}

