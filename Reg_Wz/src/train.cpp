#include"..\include\define.h"
#include<opencv.hpp>
#include"..\include\train.h"
#include"..\include\getData.h"
#include"..\include\getFeature.h"
#include"..\include\predict.h"

using namespace cv;

int train(string data_path)
{
	Mat train_data_mat;		//ͼ������������
	Mat labels_mat;			//��Ӧ��ͼ���������

	train_data_mat.convertTo(train_data_mat, CV_32F);
	labels_mat.convertTo(labels_mat, CV_32FC1);

	/*BP ģ�ʹ����Ͳ�������*/
	Ptr<ml::ANN_MLP> bp = ml::ANN_MLP::create();

	Mat layers_size = (Mat_<int>(1, 6) << 57, 57, 36, 20, 10, 10);
	bp->setLayerSizes(layers_size);

	bp->setTrainMethod(ml::ANN_MLP::BACKPROP, 0.1, 0.005);
	bp->setActivationFunction(ml::ANN_MLP::SIGMOID_SYM);
	bp->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 10000, 0.05));

	GetData(data_path, train_data_mat, labels_mat);		//��ȡͼ����������������Ӧ��ͼ���������


	/* ����ѵ���õ����������*/
	cout << "Start training........." << endl;
	bool trained = bp->train(train_data_mat, ml::ROW_SAMPLE, labels_mat);
	if (trained) {
		cout << "Training is over!!!!!!" << endl;
		bp->save("param.xml");
		return 0;
	}
	else
	{
		cout << "Training failed!!!!!!" << endl;
		return -1;
	}

}

float test(string path)
{
	long correct_num = 0, total_num = 0;
	long cor_num_class = 0, total_num_class = 0;
	float correct_rate = 0.0f;
	ANN_Wz _ann = ANN_Wz();
	int result;

	intptr_t	hFile1 = 0;
	intptr_t	hFile2 = 0;
	struct _finddata_t fileinfo;  //����ļ���Ϣ;

	string p;
	string subDir;  //��Ŀ¼·������
	string fileName;	//�ļ�Ŀ¼����

	int classNum = 0;

	cout << "Start testing the test set...." << endl;

	if ((hFile1 = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib == _A_SUBDIR)) //�ж��Ƿ�Ϊ�ļ���
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) //�������ѵ�����ļ���
				{		
					cor_num_class = 0;
					total_num_class = 0;
					classNum = strToInt(fileinfo.name);	

					hFile2 = 0;
					subDir.assign(path).append("\\").append(fileinfo.name);  //��Ŀ¼·��																			
					if ((hFile2 = _findfirst(p.assign(subDir).append("\\*").c_str(), &fileinfo)) != -1)		// ��ȡ��Ŀ¼�µ�һ���ļ����
					{
						do
						{
							if ((fileinfo.attrib != _A_SUBDIR))  //�ļ�Ϊͼ���ļ�
							{								
								fileName.assign(subDir).append("\\").append(fileinfo.name); //�ļ�����·��+�ļ���
								Mat temp_img = imread(fileName, 0);
								if (NULL==temp_img.data){
									continue;
								}
								result = _ann.predict(temp_img);	//����ѵ���õ�������Ԥ����

								total_num++;
								total_num_class++;
								if (result == classNum)
								{
									correct_num++;
									cor_num_class++;
								}
								else
								{
									//cout << fileName << endl;
								}

							}
						} while (_findnext(hFile2, &fileinfo) == 0);  //Ѱ����һ���ļ�
					}

					correct_rate = float(cor_num_class) / float(total_num_class);//ÿһ����ȷ�ʵļ���
					cout << "Character:   " << classNum << "     " << correct_rate << endl << endl << endl;

				}
			}
		} while (_findnext(hFile1, &fileinfo) == 0);  //Ѱ����һ�����ɹ�����0������-1
	}

	correct_rate = float(correct_num) / float(total_num);//ÿһ����ȷ�ʵļ���
	cout << "Correct Rate:   " << correct_rate << endl;

	return correct_rate;
}
