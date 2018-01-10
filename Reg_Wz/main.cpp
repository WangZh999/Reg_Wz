#include"include\seg.h"

void main()
{
	Mat src = imread("img/15.jpg", 0);
	Mat img;
	preProcessing_wz(src, img);
	imshow("th", img);
	Mat flag;
	vector<cv::Point>seeds;
	seeds = connection_areas_4(img, flag);
	imshow("flag", flag);
	cout << seeds.size() << endl;
	for (int i = 0; i < seeds.size(); i++)
	{
		draw_areas(flag, Mat(), i + 1);
	}
	waitKey();
}