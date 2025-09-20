#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <arcsoft_face_sdk.h>

#include "Camera.h"

#define FaceNums 1
#define Feature_Len 1032
#define Feature_Size 258

struct FacePoint
{
	float* feature;
	int faceID;
};

class FaceEngine
{
	friend void SaveNewFace();
private:
	void ReadFile();
	double CountDistance(float* point, float* target);
public:
	FaceEngine();
	~FaceEngine();
	//MRECT转cv::Rect
	cv::Rect MRECT_to_Rect(MRECT mrect);
	//初始化特征向量
	void InitASFFeature(std::vector<ASF_FaceFeature>& faceFeature, int sizeNums = FaceNums);

	//人脸识别线程函数
	void EngineThread(bool& run, Camera& cam);

	//人脸识别
	void FaceDetect(ASF_MultiFaceInfo& detectFace, cv::Mat& os, int faceNums = FaceNums);
	//特征提取
	void FaceFeature(std::vector<ASF_FaceFeature>& faceFeature, cv::Mat& os, ASF_MultiFaceInfo detectFace);
	//人脸比对
	int FaceCompare(ASF_FaceFeature feature);
private:
	MHandle handle;
	cv::VideoCapture cap;
	//存储关联信息
	std::vector<std::string> faceInfo;
	//存储人脸特征
	std::vector<FacePoint> faceData;
};

