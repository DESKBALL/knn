#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <opencv2/opencv.hpp>

struct FaceRect
{
	cv::Rect	rect;	//��������Ϣ
	std::string info;	//����������Ϣ
};

class Camera
{
public:
	Camera();
	~Camera();

	//���뻺�����
	void InputBufferQueue(std::vector<FaceRect> Queue);
	//����ͷ�̺߳���
	void RunCamera(bool& run);
	//��ʾ����ͼƬ
	void ShowImage(cv::Mat& image, bool& keep);
private:
	std::mutex mtx;
	cv::VideoCapture cap;
	//�������
	std::vector<FaceRect> RectBuffer;
};

