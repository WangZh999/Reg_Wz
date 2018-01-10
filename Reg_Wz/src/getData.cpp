#include"..\include\train.h"
#include"..\include\getData.h"
#include"..\include\getFeature.h"


bool GetData(string path, Mat &trainData, Mat &labelData)
{
	//�ļ����  
	intptr_t	hFile1 = 0;
	intptr_t	hFile2 = 0;

	struct _finddata_t fileinfo;  //����ļ���Ϣ;

	string p;
	string subDir;  //��Ŀ¼·������
	string fileName;	//�ļ�Ŀ¼����

	int classNum = 0;

	cout << "Get the image feature vectors...." << endl;

	if ((hFile1 = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib == _A_SUBDIR)) //�ж��Ƿ�Ϊ�ļ���
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					classNum = strToInt(fileinfo.name);
					Mat temp_classNum = (Mat_<float>(1, CLASS_NUM));
					for (int i = 0; i < CLASS_NUM; i++)
					{
						if (i == classNum)
						{
							temp_classNum.at<float>(i) = 1.0f;
						}
						else
						{
							temp_classNum.at<float>(i) = 0.0f;
						}
					}
					cout << "Character:    " << fileinfo.name << endl;
					

					hFile2 = 0;
					subDir.assign(path).append("\\").append(fileinfo.name);  //��Ŀ¼·��


					/*��ȡ��Ŀ¼�µ�һ���ļ����*/
					if ((hFile2 = _findfirst(p.assign(subDir).append("\\*").c_str(), &fileinfo)) != -1)
					{
						do
						{
							if ((fileinfo.attrib != _A_SUBDIR))  //�ļ�Ϊͼ���ļ�
							{
								fileName.assign(subDir).append("\\").append(fileinfo.name); //�ļ�����·��+�ļ���
								Mat temp_img = imread(fileName, 0);
								if (NULL != temp_img.data)
								{
									Mat temp_feature = getFeature(temp_img); //��ȡͼ����������
									trainData.push_back(temp_feature);
									labelData.push_back(temp_classNum);
								}
							}
						} while (_findnext(hFile2, &fileinfo) == 0);  //Ѱ����һ���ļ�
					}
				}
			}
		} while (_findnext(hFile1, &fileinfo) == 0);  //Ѱ����һ�����ɹ�����0������-1
	}

	return true;
}



/*********************
0-9: 0-9
10-35: A-Z
*********************/
int strToInt(string str)
{
	int num;
	char c = str.c_str()[0];
	if (c < 'A')
	{
		num = c - '0';
	}
	else
	{
		num = c - 'A' + 10;
	}

	return num;
}