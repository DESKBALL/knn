#pragma once

#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <arcsoft_face_sdk.h>

#define APPID (char*)"4UVZzzZSKsWwEBMuKrBFyfqhYVmAUGFwFDmUik9kDJDN"
#define SDKKey (char*)"CVZRULqDSFRRLpd3nxW7TYH2Zjmxayd4WAzYS8Yb3vHP"
#define FeatureNums 512
#define MaxFaceNum 1


/*----------------------------------------------------------------------------*/

class input_facedata
{
private:
	cv::VideoCapture cap;						// 摄像头
	

public:
	vector<ASF_FaceFeature> Data;				// 即将写入的数据

	//初始化变量
	input_facedata(
		int index = 0							// 摄像头ID
	);

	//释放占用
	~input_facedata();							

	//输出摄像头画面
	void showimage(
		bool& run								//控制摄像头开启
	);

	//捕获人脸
	bool dectectface();

	//特征提取
	bool facefeature(
		ASVLOFFSCREEN offscreen,				// 图像信息
		ASF_MultiFaceInfo face					// 人脸信息
	);

	//保存数据
	void CreatFaceData(
		string filename							// 训练集标签
	);
};

