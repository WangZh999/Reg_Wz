#include".\include\define.h"
#include"test.h"

void test1(string fileName)
{
	ANN_Wz _ann = ANN_Wz("param.xml");
	Mat src = imread(fileName, 0);
	if (NULL == src.data)
	{
		cout << "img do not exist!" << endl;
		return;
	}
	Mat img;
	preProcessing_wz(src, img);
	imshow("th", img);

	Mat flag;
	vector<cv::Point>seeds;
	seeds = connection_areas_4(img, flag);

	for (int i = 0; i < seeds.size(); i++)
	{
		Mat _temp;
		draw_areas(flag, _temp, i + 1);
		int re = _ann.predict(_temp);
		cout << re << endl;
		imshow("temp", _temp);
		cv::waitKey(0);
		destroyWindow("temp");
	}
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void test2(string path_b)
{
	ANN_Wz _ann = ANN_Wz("param.xml");

	intptr_t hFile = 0;
	struct _finddata_t fileInfo;

	string path;
	string path_file;

	int k = 0;

	if ((hFile = _findfirst(path.assign(path_b).append("\\*.jpg").c_str(), &fileInfo)) != -1) {
		while (k != -1) {
			path_file.assign(path_b).append("\\").append(fileInfo.name);			
			Mat _src = imread(path_file, 0);
			if (NULL == _src.data)
			{
				cout << "img do not exist!" << endl;
				continue;
			}

			Mat _img;
			preProcessing_wz(_src, _img, true);
			imshow("th", _img);

			Mat _flag;
			vector<cv::Point>seeds;
			seeds = connection_areas_4(_img, _flag);

			for (int i = 0; i < seeds.size(); i++)
			{
				Mat _temp;
				draw_areas(_flag, _temp, i + 1);
				int re = _ann.predict(_temp);
				cout << re << endl;
				
				//string nnnn;
				//nnnn.assign("out\\").append(fileInfo.name);
				//nnnn.erase(nnnn.end() - 4, nnnn.end());
				//nnnn.append("_").append(to_string(i)).append(".jpg");
				//imwrite(nnnn, _temp);
				
				imshow("temp", _temp);
				cv::waitKey(0);
 				destroyWindow("temp");
			}
			
			k = _findnext(hFile, &fileInfo);
		}
	}
}

void test3(string path_b)
{
	intptr_t hFile = 0;
	struct _finddata_t fileInfo;

	string path;
	string path_file;

	int k = 0;

	uchar value;
	int value_new;


	if ((hFile = _findfirst(path.assign(path_b).append("\\*.jpg").c_str(), &fileInfo)) != -1) {
		while (k != -1) {
			path_file.assign(path_b).append("\\").append(fileInfo.name);
			Mat _src = imread(path_file, 0);
			if (NULL == _src.data)
			{
				cout << "img do not exist!" << endl;
				continue;
			}

			resize(_src, _src, Size(500, 500));
			Mat _img(_src.size(), CV_8UC1);
			//bilateralFilter(src, img, 5, 100, 100);
			blur(_src, _img, Size(5, 5));
			medianBlur(_src, _img, 5);

			imshow("src", _img);

			Mat imageEnhance(_img.rows, _img.cols, CV_8UC1, Scalar(0));
			//equalizeHist(_img, imageEnhance);//ֱ��ͼ���⻯
			Mat kernel = (Mat_<float>(3, 3) << 0, -2, 0, 0, 6, 0, 0, -2, 0);
			//Mat kernel = (Mat_<float>(3, 3) << -1, -2, -1, 0, 17, 0, -1, -2, -1);
			//filter2D(_img, _img, CV_8UC1, kernel);//������˹����ͼ����ǿ

		/*	for (int _i = 0; _i < _img.rows; _i++)
			{
				uchar * ptr_s = _img.ptr(_i);
				uchar * ptr_d = imageEnhance.ptr(_i);
				for (int _j = 0; _j < _img.cols; _j++)
				{
					value = ptr_s[_j];
					ratio = value / 255.0;
					ptr_d[_j] = (uchar)value*ratio*ratio*ratio;
				}
			}*/

			//imshow("lap", _img);


			for (int _i = 0; _i < _img.rows; _i++)
			{
				uchar * ptr_s = _img.ptr(_i);
				uchar * ptr_d = imageEnhance.ptr(_i);
				for (int _j = 0; _j < _img.cols; _j++)
				{
					value = ptr_s[_j];
					if (value < 60) {
						value_new = (int)value * 0.3;
					}
					else if (value < 100) {
						value_new = value - 42;
					}
					else if (value < 170) {
						value_new = value * 2 - 142;
					}
					else {
						value_new = 200;
					}
					ptr_d[_j] = (uchar)value_new;
				}
			}

			
			imshow("enhance", imageEnhance);

			threshold(imageEnhance, imageEnhance, 0, 255, THRESH_OTSU);
			threshold(imageEnhance, imageEnhance, 80, 255, THRESH_BINARY_INV);
			Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 13));
			erode(imageEnhance, imageEnhance, element);

			imshow("th", imageEnhance);

			waitKey(0);
			destroyAllWindows();

			k = _findnext(hFile, &fileInfo);
		}
	}
}

void test4(string path_b)
{
	intptr_t hFile = 0;
	struct _finddata_t fileInfo;

	string path;
	string path_file;

	int k = 0;

	string path_img; //= "E:/REG_Wz/Reg_Wz/train/";

	int img_value;

	if ((hFile = _findfirst(path.assign(path_b).append("\\*.jpg").c_str(), &fileInfo)) != -1) {
		while (k != -1) {
			path_file.assign(path_b).append("\\").append(fileInfo.name);
			Mat _src = imread(path_file, 0);
			if (NULL == _src.data)
			{
				cout << "img do not exist!" << endl;
				continue;
			}
			
			imshow("img", _src);
			img_value = waitKey();
			img_value -= 48;

			path_img.assign("E:/REG_Wz/Reg_Wz/train/").append(to_string(img_value)).append("/").append(fileInfo.name);
			
			cout << path_img << endl;

			if (imwrite(path_img, _src)) {
				remove(path_file.c_str());
			}


			k = _findnext(hFile, &fileInfo);
		}
	}
}

void test5(string path_b)
{
	intptr_t hFile = 0;
	struct _finddata_t fileInfo;

	string path;
	string path_file;

	int k = 0;

	if ((hFile = _findfirst(path.assign(path_b).append("\\*.png").c_str(), &fileInfo)) != -1) {
		while (k != -1) {
			path_file.assign(path_b).append("\\").append(fileInfo.name);
			Mat _src = imread(path_file, 0);
			if (NULL == _src.data)
			{
				cout << "img do not exist!" << endl;
				continue;
			}
			cout << path_file << endl;
			int _re = recognition(_src);
			cout << _re << endl;
			waitKey(0);
			destroyAllWindows();
			
			k = _findnext(hFile, &fileInfo);
		}
	}
}



void test6(string path_b)
{
	intptr_t hFile = 0;
	struct _finddata_t fileInfo;

	string path;
	string path_file;

	int k = 0;

	if ((hFile = _findfirst(path.assign(path_b).append("\\*.png").c_str(), &fileInfo)) != -1) {
		while (k != -1) {
			path_file.assign(path_b).append("\\").append(fileInfo.name);
			Mat _src = imread(path_file, 0);
			if (NULL == _src.data)
			{
				cout << "img do not exist!" << endl;
				continue;
			}
			cout << path_file << endl;
			//threshold(_src, _src, 100, 255, THRESH_BINARY_INV);
			int _re = recognition(_src);
			cout << _re << endl;
			waitKey(0);
			destroyAllWindows();

			k = _findnext(hFile, &fileInfo);
		}
	}
}



void test7(string path_b)
{
	intptr_t hFile = 0;
	struct _finddata_t fileInfo;

	string path;
	string path_file;

	int k = 0;

	if ((hFile = _findfirst(path.assign(path_b).append("\\*.png").c_str(), &fileInfo)) != -1) {
		while (k != -1) {
			path_file.assign(path_b).append("\\").append(fileInfo.name);
			Mat _src = imread(path_file, 0);
			if (NULL == _src.data)
			{
				cout << "img do not exist!" << endl;
				continue;
			}

			Mat dst;

			resize(_src, _src, Size(100, 100));
			Mat img(_src.size(), CV_8UC1);
			blur(_src, img, Size(5, 5));
			medianBlur(img, dst, 5);

			vector<Vec3f> circles;
			HoughCircles(dst, circles, HOUGH_GRADIENT,
				1, 10, 100, 50, dst.rows / 20, dst.rows / 2);
			
			cout << path_file << " :   " << circles.size() << endl;

			resize(dst, dst, Size(500, 500));

			for (size_t i = 0; i < circles.size(); i++)
			{
				Point center(cvRound(circles[i][0] * 5), cvRound(circles[i][1] * 5));
				int radius = cvRound(circles[i][2] * 5);
				// draw the circle center
				circle(dst, center, 3, Scalar(0), -1, 8, 0);
				// draw the circle outline
				circle(dst, center, radius, Scalar(0), 8, 8, 0);
			}
			namedWindow("circles", 1);
			imshow("circles", dst);
			waitKey(0);
			destroyAllWindows();

			k = _findnext(hFile, &fileInfo);
		}
	}
}