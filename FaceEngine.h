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
	//MRECTתcv::Rect
	cv::Rect MRECT_to_Rect(MRECT mrect);
	//��ʼ����������
	void InitASFFeature(std::vector<ASF_FaceFeature>& faceFeature, int sizeNums = FaceNums);

	//����ʶ���̺߳���
	void EngineThread(bool& run, Camera& cam);

	//����ʶ��
	void FaceDetect(ASF_MultiFaceInfo& detectFace, cv::Mat& os, int faceNums = FaceNums);
	//������ȡ
	void FaceFeature(std::vector<ASF_FaceFeature>& faceFeature, cv::Mat& os, ASF_MultiFaceInfo detectFace);
	//�����ȶ�
	int FaceCompare(ASF_FaceFeature feature);
private:
	MHandle handle;
	cv::VideoCapture cap;
	//�洢������Ϣ
	std::vector<std::string> faceInfo;
	//�洢��������
	std::vector<FacePoint> faceData;
};

