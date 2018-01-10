#include"include\seg.h"
#include"include\predict.h"

void main()
{
	ANN_Wz _ann = ANN_Wz("w.xml");
	Mat src = imread("img/15.jpg", 0);
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
		waitKey(0);
		destroyWindow("_temp");
	}
}