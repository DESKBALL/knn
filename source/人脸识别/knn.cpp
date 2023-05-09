
#include <io.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <arcsoft_face_sdk.h>

using namespace std;
using namespace cv;

#include "detectface.h"

size_t typeNums;
size_t Nums;

//计算欧式距离
double CountDistance(float* target, float* point, size_t len = 258)
{
	double D = 0.0f;
	for (size_t i = 0; i < len; i++)
	{
		//计算
		D = D + pow(target[i] - point[i], 2);
	}

	return sqrt(D);
}


void knn(float* face)
{
	typeNums = _file_name_.size();
	Nums = _train_data_.size();

	vector<pair<double,int>> distance;	//储存距离参数
	for (size_t i = 0; i < Nums; i++)
	{
		float* facepoint = _train_data_[i].feature;				//取出样本
		double D = CountDistance(face, facepoint);				//计算欧式距离
		distance.push_back(make_pair(D,_train_data_[i].type));	//添加到容器中
	}	


	//k近邻
	const int K = 3;	//定义算法K值为3
	int Maxpos = 0;		//最多样本
	//count记录前K个样本中各个类别出现次数与该类别距离最近样本
	vector<pair<size_t,double>> count(typeNums);
	//初始化count
	for (size_t i = 0; i < typeNums; i++)
	{
		count[i].first = 0;
		count[i].second = SIZE_MAX;
	}

	//排序
	sort(distance.begin(), distance.end());
	//计算最近K个样本类别出现次数
	for (size_t i = 0; i < K && i < Nums; i++)
	{
		count[distance[i].second].first+=1;
		if (count[distance[i].second].second > distance[i].first) {
			count[distance[i].second].second = distance[i].first;
		}
	}
	//找到最多类别
	for (size_t i = 0; i < typeNums; i++)
	{
		if (count[i].first > count[Maxpos].first) {
			Maxpos = i;
		}
	}

	double dis = count[Maxpos].second;
	printf("%.2lf\n", dis);
	if (dis < 1.0f)
		Mpos = Maxpos;
	else
		Mpos = -1;
	
	count.clear();
}