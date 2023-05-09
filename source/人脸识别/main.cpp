
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <windows.h>

#include<opencv2/opencv.hpp>
#include<arcsoft_face_sdk.h>

using namespace std;
using namespace cv;

#include "detectface.h"


int main()
{
	//初始化摄像头
	int index = 0;
	VideoCapture cap = VideoCapture(index);

	//读取训练集
	readtraindata();

	//调用线程
	mutex mtx;								// 声明互斥锁
	bool runThread = true;					// 控制线程运行
	ASF_MultiFaceInfo faceRect = { 0 };		// 存储人脸参数
	thread detectFace(DetectFace,
		ref(cap), ref(runThread), ref(faceRect), ref(mtx)); //调用线程

	//显示画面
	Mat os;
    while (true)
    {
		//将摄像头画面流入Mat类中 
		cap >> os;
        {
            lock_guard<mutex> mlock(mtx);
			
			Rect cvRect; 

			if (faceRect.faceRect && Mpos >= 0 && Mpos < _file_name_.size())
			{
				//MRECT转RECT
				cvRect.x = faceRect.faceRect->left;
				cvRect.y = faceRect.faceRect->top;
				cvRect.width = faceRect.faceRect->right - faceRect.faceRect->left;
				cvRect.height = faceRect.faceRect->bottom - faceRect.faceRect->top;

				//框出人脸
				rectangle(os, cvRect, Scalar(0x02, 0x03, 0xFE), 2);

				//打印关联信息
				string faceInfo=_file_name_[Mpos];
				putText(os, faceInfo, Point(cvRect.x, cvRect.y + cvRect.height + 25),
					FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0x02, 0x03, 0xFE), 2);
			}
        }

        //显示画面
        imshow("detectfaces",os);

        if (waitKey(1) == '\r') {
            break;
        }

        //休眠
        this_thread::sleep_for(chrono::milliseconds(40));
    }

	//释放训练库占用空间
	for (auto& freedata : _train_data_)
	{
		free(freedata.feature);
	}

    //退出线程
    runThread = false;
    detectFace.join();
    return 0;
}