#include"thin.h"

using namespace std;

void chao_thinimage(Mat &srcimage)//单通道、二值化后的图像
{
	vector<Point> deletelist1;
	int _flag[9];
	int nl = srcimage.rows;
	int nc = srcimage.cols;
	while (true)
	{
		for (int j = 1; j < (nl - 1); j++)
		{
			uchar* data_last = srcimage.ptr<uchar>(j - 1);
			uchar* data = srcimage.ptr<uchar>(j);
			uchar* data_next = srcimage.ptr<uchar>(j + 1);
			for (int i = 1; i < (nc - 1); i++)
			{
				if (data[i] == 255)
				{
					_flag[0] = 1;
					if (data_last[i] == 255) _flag[1] = 1;
					else  _flag[1] = 0;
					if (data_last[i + 1] == 255) _flag[2] = 1;
					else  _flag[2] = 0;
					if (data[i + 1] == 255) _flag[3] = 1;
					else  _flag[3] = 0;
					if (data_next[i + 1] == 255) _flag[4] = 1;
					else  _flag[4] = 0;
					if (data_next[i] == 255) _flag[5] = 1;
					else  _flag[5] = 0;
					if (data_next[i - 1] == 255) _flag[6] = 1;
					else  _flag[6] = 0;
					if (data[i - 1] == 255) _flag[7] = 1;
					else  _flag[7] = 0;
					if (data_last[i - 1] == 255) _flag[8] = 1;
					else  _flag[8] = 0;
					int whitepointtotal = 0;
					for (int k = 1; k < 9; k++)
					{
						whitepointtotal = whitepointtotal + _flag[k];
					}
					if ((whitepointtotal >= 2) && (whitepointtotal <= 6))
					{
						int ap = 0;
						if ((_flag[1] == 0) && (_flag[2] == 1)) ap++;
						if ((_flag[2] == 0) && (_flag[3] == 1)) ap++;
						if ((_flag[3] == 0) && (_flag[4] == 1)) ap++;
						if ((_flag[4] == 0) && (_flag[5] == 1)) ap++;
						if ((_flag[5] == 0) && (_flag[6] == 1)) ap++;
						if ((_flag[6] == 0) && (_flag[7] == 1)) ap++;
						if ((_flag[7] == 0) && (_flag[8] == 1)) ap++;
						if ((_flag[8] == 0) && (_flag[1] == 1)) ap++;
						if (ap == 1)
						{
							if ((_flag[1] * _flag[7] * _flag[5] == 0) && (_flag[3] * _flag[5] * _flag[7] == 0))
							{
								deletelist1.push_back(Point(i, j));
							}
						}
					}
				}
			}
		}
		if (deletelist1.size() == 0) break;
		for (size_t i = 0; i < deletelist1.size(); i++)
		{
			Point tem;
			tem = deletelist1[i];
			uchar* data = srcimage.ptr<uchar>(tem.y);
			data[tem.x] = 0;
		}
		deletelist1.clear();

		for (int j = 1; j < (nl - 1); j++)
		{
			uchar* data_last = srcimage.ptr<uchar>(j - 1);
			uchar* data = srcimage.ptr<uchar>(j);
			uchar* data_next = srcimage.ptr<uchar>(j + 1);
			for (int i = 1; i < (nc - 1); i++)
			{
				if (data[i] == 255)
				{
					_flag[0] = 1;
					if (data_last[i] == 255) _flag[1] = 1;
					else  _flag[1] = 0;
					if (data_last[i + 1] == 255) _flag[2] = 1;
					else  _flag[2] = 0;
					if (data[i + 1] == 255) _flag[3] = 1;
					else  _flag[3] = 0;
					if (data_next[i + 1] == 255) _flag[4] = 1;
					else  _flag[4] = 0;
					if (data_next[i] == 255) _flag[5] = 1;
					else  _flag[5] = 0;
					if (data_next[i - 1] == 255) _flag[6] = 1;
					else  _flag[6] = 0;
					if (data[i - 1] == 255) _flag[7] = 1;
					else  _flag[7] = 0;
					if (data_last[i - 1] == 255) _flag[8] = 1;
					else  _flag[8] = 0;
					int whitepointtotal = 0;
					for (int k = 1; k < 9; k++)
					{
						whitepointtotal = whitepointtotal + _flag[k];
					}
					if ((whitepointtotal >= 2) && (whitepointtotal <= 6))
					{
						int ap = 0;
						if ((_flag[1] == 0) && (_flag[2] == 1)) ap++;
						if ((_flag[2] == 0) && (_flag[3] == 1)) ap++;
						if ((_flag[3] == 0) && (_flag[4] == 1)) ap++;
						if ((_flag[4] == 0) && (_flag[5] == 1)) ap++;
						if ((_flag[5] == 0) && (_flag[6] == 1)) ap++;
						if ((_flag[6] == 0) && (_flag[7] == 1)) ap++;
						if ((_flag[7] == 0) && (_flag[8] == 1)) ap++;
						if ((_flag[8] == 0) && (_flag[1] == 1)) ap++;
						if (ap == 1)
						{
							if ((_flag[1] * _flag[3] * _flag[5] == 0) && (_flag[3] * _flag[1] * _flag[7] == 0))
							{
								deletelist1.push_back(Point(i, j));
							}
						}
					}
				}
			}
		}
		if (deletelist1.size() == 0) break;
		for (size_t i = 0; i < deletelist1.size(); i++)
		{
			Point tem;
			tem = deletelist1[i];
			uchar* data = srcimage.ptr<uchar>(tem.y);
			data[tem.x] = 0;
		}
		deletelist1.clear();
	}

}
