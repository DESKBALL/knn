//提取人脸特征
#include <iostream>
#include <opencv2/opencv.hpp>
#include <arcsoft_face_sdk.h>

using namespace std;
using namespace cv;

#include "detectface.h"


//从外部传入图像，Rect参数
int ArcFace(ASVLOFFSCREEN offscreen, ASF_MultiFaceInfo face)
{
	//利用ASFFaceFeatureExtractEx();
	// 需要 1.引擎句柄  2.图像信息(ASVLOFFSCREEN结构体) 3.人脸位置(ASF_SingleFaceInfo)
	//一个ASF_FaceFeatrue类用于存储输出
	 

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
	ASF_FaceFeature face_features = {0};
	res = ASFFaceFeatureExtractEx(handle, &offscreen, &signedfaceinfo, &face_features);
	if (res != 0) {
		printf("Error: failed to detect. stop(0x%08x)\n",res);
		return -1;
	}
	else
	{	//如果成功，输出特征值
		size_t feature_len = face_features.featureSize / sizeof(float);
		float* Vfeature = (float*)face_features.feature;
		
		system("cls");
		printf("采集到特征:%llu, 特征值：\n",face_features.featureSize/sizeof(float));
		for (size_t i = 0; i < feature_len; i++)
		{	
			printf("%f, ",Vfeature[i]);
		}
		putchar('\n');

		knn(Vfeature);
		
	}


	//释放引擎占用的内存空间
	ASFUninitEngine(handle);
	handle = NULL;
	

    return 0;
}