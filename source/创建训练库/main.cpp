
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <fstream>
#include <windows.h>

#include <opencv2/opencv.hpp>
#include <arcsoft_face_sdk.h>

using namespace std;
using namespace cv;

#include "input_facedata.h"

input_facedata facecheck;	//创建人脸识别对象

int main()
{
	//显示摄像头画面
	bool run;
	thread camera(&input_facedata::showimage, &facecheck, ref(run));

	//样本数
	this_thread::sleep_for(chrono::seconds(1));
	system("cls");
	int Nums;
	printf("请输入要写入样本的数量:");
	cin >> Nums;


	//获取关联信息
	string filename,name, sex, age;
	int countSex;
	printf("请输入关联信息\n");
	printf("姓名:"); cin >> name;
	do {
		printf("性别 (1. 男 / 0. 女) :");
		cin >> countSex;
		if		(countSex == 1)	sex = "mam";
		else if (countSex == 0) sex = "womem";
		else	cout << "请不要开玩笑! 你不可能是武装直升机!\n";
	} while (countSex != 1 && countSex != 0);
	printf("年龄:"); cin >> age;
	filename = name + ' ' + sex + ' ' + age;
	system("pause");


	while (Nums--)
	{
		facecheck.dectectface();
		system("pause");

		
	}

	facecheck.CreatFaceData(filename);
	run = false;
	camera.join();


    return 0;
}