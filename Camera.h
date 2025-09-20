#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <opencv2/opencv.hpp>

struct FaceRect
{
	cv::Rect	rect;	//人脸框信息
	std::string info;	//人脸关联信息
};

class Camera
{
public:
	Camera();
	~Camera();

	//输入缓存队列
	void InputBufferQueue(std::vector<FaceRect> Queue);
	//摄像头线程函数
	void RunCamera(bool& run);
	//显示单张图片
	void ShowImage(cv::Mat& image, bool& keep);
private:
	std::mutex mtx;
	cv::VideoCapture cap;
	//缓存队列
	std::vector<FaceRect> RectBuffer;
};

