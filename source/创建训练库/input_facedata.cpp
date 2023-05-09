
#include <iostream>
#include <fstream>
#include <cstring>

#include <opencv2/opencv.hpp>
#include <arcsoft_face_sdk.h>

using namespace std;
using namespace cv;

#include "input_facedata.h"


input_facedata::input_facedata
	(int index){
	
	//初始化摄像头
	cap = VideoCapture(index, CAP_ANY);
	if (!cap.isOpened()) {
		printf("Error: failed to VideoCaptrue.\n");
		exit(1);
	}


	//认证Arcsoft证书
	MRESULT res = ASFOnlineActivation(APPID, SDKKey);
	if (res != 0 /*完成*/ && res != 0x16002 /*SDK已激活*/) {
		printf("Error: failed to activate license. stop(0x%08x)", res);
		exit(1);
	}

}


input_facedata::~input_facedata() {

	cap.release();          // 释放已使用的 VideoCapture 对象
	destroyAllWindows();    // 关闭所有打开的 OpenCV 窗口
}



void input_facedata::showimage(bool& run) 
{
	Mat os;
	while (run)
	{
		cap >> os;
		imshow("摄像头", os);		
		waitKey(1);
		this_thread::sleep_for(chrono::microseconds(40));
	}
}


bool input_facedata::dectectface()
{
	//初始化引擎
	//用于人脸检测的引擎
	MHandle handle = NULL;
	MRESULT res = ASFInitEngine(ASF_DETECT_MODE_IMAGE, ASF_OP_0_ONLY,
		32, MaxFaceNum, ASF_FACE_DETECT | ASF_FACERECOGNITION, &handle);
	if (res != 0) {
		printf("Error: failed to initialize Engine. stop(0x%08x)\n", res);
		return false;
	}

	//获取图像
	Mat os;
	cap >> os;
	if (os.empty()) {
		printf("Error: failed to cap. \n");
		return false;
	}

	//存入数据
	ASVLOFFSCREEN offscreen = { 0 };
	offscreen.i32Width = os.cols;
	offscreen.i32Height = os.rows;
	offscreen.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;;
	offscreen.pi32Pitch[0] = os.step;
	offscreen.ppu8Plane[0] = os.data;

	//捕捉人脸
	bool is_getfeatrue = false;
	ASF_MultiFaceInfo faceRect = { 0 };
	do{
		//循环直到成功捕获人脸
		res = ASFDetectFacesEx(handle, &offscreen, &faceRect);
		if (res == 0) {
			//检测到人脸后调用函数提取特征
			is_getfeatrue = facefeature(offscreen, faceRect);
		}
	} while (res != 0 || !is_getfeatrue);
	
	//释放资源
	ASFUninitEngine(handle);
	handle = NULL;
	return true;
}


bool input_facedata::facefeature(
	ASVLOFFSCREEN offscreen, ASF_MultiFaceInfo face)
{
	//初始化图像引擎
	//设置模式为特征提取
	MHandle handle = NULL;
	MRESULT res = ASFInitEngine(ASF_DETECT_MODE_IMAGE, ASF_OP_0_ONLY,
		32, MaxFaceNum, ASF_FACERECOGNITION | ASF_FACE_DETECT, &handle);
	if (res != 0) {
		printf("Error: failed to initialize Engine. stop(0x%08x)\n", res);
		return -1;
	}

	//将Rect数据存入容器
	ASF_SingleFaceInfo signedfaceinfo;
	signedfaceinfo.faceRect = *face.faceRect;
	signedfaceinfo.faceOrient = *face.faceOrient;

	//创建容器
	//开始提取特征
	ASF_FaceFeature face_features = { 0 };
	res = ASFFaceFeatureExtractEx(handle, &offscreen, &signedfaceinfo, &face_features);
	if (res != 0) {
		printf("Error: failed to detect. stop(0x%08x)\n", res);
		return false;
	}
	else
	{	//如果成功，打印输出特征值
		float* Vfeature = (float*)face_features.feature;
		size_t lenght = face_features.featureSize / sizeof(float);

		system("cls");
		printf("捕获成功!\n");
		for (size_t i = 0; i < lenght; i++)
		{
			printf("%f ",Vfeature[i]);
		}
		putchar('\n');

		//存储数据
		ASF_FaceFeature buffer = { 0 };
		buffer.feature = (MByte*)calloc(1,face_features.featureSize);
		memcpy(buffer.feature, face_features.feature, face_features.featureSize);
		buffer.featureSize = face_features.featureSize;
		
		//添加到缓存队列中
		Data.push_back(buffer);
	}

	//释放引擎占用的内存空间
	ASFUninitEngine(handle);
	handle = NULL;
	return true;
}

void input_facedata::CreatFaceData(string filename)
{
	//创建训练集文件
	filename = "bin\\" + filename + ".dat";
	ofstream writeData(filename, ios::app | ios::binary);

	size_t filesize = 0;	// 记录写入字节数

	//写入
	for (auto& face : Data)
	{
		//计算数量
		size_t feature_len = face.featureSize / sizeof(float);

		//样本开头添加特征占用大小
		writeData.write((char*)&face.featureSize, sizeof(int));

		//写入样本数据
		writeData.write((char*)face.feature, face.featureSize);

		//记录写入数据量
		filesize = filesize + sizeof(int) + face.featureSize;

		//释放内存
		free(face.feature);
	}

	writeData.close();
	Data.clear();
	system("cls");
	printf("完成, 总共写入字节数: %llu\n",filesize);
}



