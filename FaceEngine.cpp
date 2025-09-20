
#include <opencv2/opencv.hpp>
#include <arcsoft_face_sdk.h>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <cmath>
#include <climits>

#include "FaceEngine.h"

using namespace std;
using namespace cv;

void FaceEngine::ReadFile()
{
	string path = "facedata\\";
	faceInfo.push_back("Unknown");
	int faceID = 1;

	//����Ŀ¼�ļ�
	for (const auto& entry : filesystem::directory_iterator(path))
	{
		//�ж��ļ���׺
		if (entry.path().extension() != ".face") {
			continue;
		}

		//���ļ�
		ifstream in_file(entry.path(), ios::binary);
		if (!in_file.is_open()) {
			printf("Error: failed to opened file!\n");
			exit(0);
		}

		int dataSize = 0;
		float* feature = nullptr;
		FacePoint buffer = { 0 };

		//��ȡ�ļ�
		while (in_file.read((char*)&dataSize, sizeof(int)))
		{
			//����ռ䴢������
			feature = (float*)malloc(dataSize);
			if (!feature) {
				printf("Error: failed to malloc memory!\n");
				exit(0);
			}

			//��ȡ����
			in_file.read((char*)feature, dataSize);

			//������Ϣ
			buffer.feature = feature;
			buffer.faceID = faceID;
			faceData.push_back(buffer);
		}

		in_file.close();

		//�洢������Ϣ
		string info = entry.path().filename().string();
		info.erase(info.size() - 5, 5);		//�޳���׺
		faceInfo.push_back(info);

		++faceID;
	}
}

//��ʼ����������
void FaceEngine::InitASFFeature(vector<ASF_FaceFeature>& faceFeature, int sizeNums)
{
	faceFeature.resize(sizeNums);

	for (size_t i = 0; i < sizeNums; i++)
	{
		faceFeature[i].feature = (MByte*)malloc(Feature_Len);
	}
}
//MRECT(Arcface)תcv::Rect
Rect FaceEngine::MRECT_to_Rect(MRECT mrect)
{
	Rect rect;
	rect.x = mrect.left;
	rect.y = mrect.top;
	rect.width = mrect.right - mrect.left;
	rect.height = mrect.bottom - mrect.top;
	return rect;
}

//����ʶ���߳�
void FaceEngine::EngineThread(bool& run, Camera& cam)
{
	Mat os;
	ASF_MultiFaceInfo detectFace = { 0 };
	vector<ASF_FaceFeature> faceFeature;
	FaceRect buffer;

	InitASFFeature(faceFeature);

	while (run)
	{
		//��������ͷ����
		cap >> os;

		FaceDetect(detectFace, os);
		FaceFeature(faceFeature, os, detectFace);
		
		vector<FaceRect> bufferQueue;
		
		for (size_t i = 0; i < detectFace.faceNum; i++)
		{
			//�ȶ�������ȡ��Ӧ�Ĺ�����Ϣ
			int faceID = FaceCompare(faceFeature[i]);

			//��������Ϣ��ӵ��������
			buffer.rect = MRECT_to_Rect(detectFace.faceRect[i]);
			buffer.info = faceInfo[faceID];
			bufferQueue.push_back(buffer);
		}
		
		cam.InputBufferQueue(bufferQueue);
	}
}


void FaceEngine::FaceDetect(ASF_MultiFaceInfo& detectFace, Mat& os, int faceNums)
{
	//opencv����ת��Arcface
	ASVLOFFSCREEN offScreen = { 0 };
	offScreen.i32Width = os.cols;
	offScreen.i32Height = os.rows;
	offScreen.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;;
	offScreen.pi32Pitch[0] = os.step;
	offScreen.ppu8Plane[0] = os.data;

	/*if (faceNums != FaceNums) {
		MHandle handle;
		ASFInitEngine(ASF_DETECT_MODE_IMAGE, ASF_OP_0_ONLY, 32, faceNums,
			ASF_FACE_DETECT | ASF_FACERECOGNITION | ASF_LIVENESS, &handle);
		ASFDetectFacesEx(handle, &offScreen, &detectFace);
		ASFUninitEngine(handle);
	}*/

	MRESULT res = ASFDetectFacesEx(handle, &offScreen, &detectFace);
	if (res != MOK) {
		//printf("No face\n");
	}
}


void FaceEngine::FaceFeature(std::vector<ASF_FaceFeature>& faceFeature, 
	cv::Mat& os, ASF_MultiFaceInfo detectFace)
{
	//opencv����ת��Arcface
	ASVLOFFSCREEN offScreen = { 0 };
	offScreen.i32Width = os.cols;
	offScreen.i32Height = os.rows;
	offScreen.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;;
	offScreen.pi32Pitch[0] = os.step;
	offScreen.ppu8Plane[0] = os.data;

	for (int i = 0; i < detectFace.faceNum; i++)
	{
		//ȡ��������
		ASF_SingleFaceInfo faceInfo = { 0 };
		faceInfo.faceRect = detectFace.faceRect[i];
		faceInfo.faceOrient = detectFace.faceOrient[i];

		MRESULT res = ASFFaceFeatureExtractEx(handle, &offScreen, &faceInfo, &faceFeature[i]);
		if (res != MOK) {
			//printf("failed to get feature\n");
		}
	}
}

//����ŷ�Ͼ���
double FaceEngine::CountDistance(float* point, float* target)
{
	double sum = 0.0f;

	for (size_t i = 0; i < Feature_Size; i++)
	{
		sum += pow(point[i] - target[i], 2);
	}

	return sqrt(sum);
}

int FaceEngine::FaceCompare(ASF_FaceFeature feature)
{
	int type = 0;
	double Dmin = DBL_MAX;
	vector<pair<float, int>> dataHash(faceInfo.size());
	
	//���������������ƽ������
	for (size_t i = 0; i < faceData.size(); i++)
	{
		dataHash[faceData[i].faceID].first += CountDistance(faceData[i].feature, (float*)feature.feature);
		dataHash[faceData[i].faceID].second++;
	}

	for (size_t i = 0; i < dataHash.size(); i++)
	{
		if (dataHash[i].first / dataHash[i].second < Dmin) {
			Dmin = dataHash[i].first / dataHash[i].second;
			type = i;
		}
	}

	/*for (size_t i = 0; i < faceData.size(); i++)
	{
		double count = CountDistance((float*)feature.feature, faceData[i].feature);
		if (Dmin > count) {
			Dmin = count;
			type = faceData[i].type;
		}
	}*/

	if (Dmin > 1.0) {
		type = 0;
	}

	//printf("%f\n", Dmin);

	return type;
}


FaceEngine::FaceEngine()
{
	ReadFile();
	cap = VideoCapture(0);
	MRESULT res = ASFInitEngine(ASF_DETECT_MODE_IMAGE, ASF_OP_0_ONLY, 32, FaceNums,
		ASF_FACE_DETECT | ASF_FACERECOGNITION | ASF_LIVENESS, &handle);
	if (MOK != res) {
		printf("Error %x: failed to initEngine!\n", res);
		exit(0);
	}
}


FaceEngine::~FaceEngine()
{
	ASFUninitEngine(handle);
	for (size_t i = 0; i < faceData.size(); i++)
	{
		free(faceData[i].feature);
	}
}


