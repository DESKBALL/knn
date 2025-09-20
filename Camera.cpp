
#include <opencv2/opencv.hpp>
#include <mutex>
#include <vector>

#include "Camera.h"

using namespace std;
using namespace cv;

Camera::Camera()
{
	cap = VideoCapture(0);
}

Camera::~Camera()
{

}

void Camera::InputBufferQueue(vector<FaceRect> Queue)
{
	lock_guard<mutex> mlock(mtx);
	RectBuffer = Queue;
}

void Camera::RunCamera(bool& run)
{
	Mat os;

	while (run)
	{
		cap >> os;

		//��黺�����
		if (mtx.try_lock()) {
			Scalar color;
			for (size_t i = 0; i < RectBuffer.size(); i++)
			{
				//δ֪Ϊ��ɫ����֪Ϊ��ɫ
				if (RectBuffer[i].info == "Unknown") {
					color = Scalar(0x14, 0x32, 0xff);
				}
				else {
					color = Scalar(0x32, 0xff, 0x14);
				}

				//����
				rectangle(os, RectBuffer[i].rect, color, 2);
				//���ƹ�����Ϣ
				Point textPoint(RectBuffer[i].rect.x,
					RectBuffer[i].rect.y + RectBuffer[i].rect.height + 20);
				putText(os, RectBuffer[i].info, textPoint, FONT_HERSHEY_DUPLEX, 0.8, color);
			}
			//RectBuffer.clear();
			mtx.unlock();
		}

		imshow("DETECTFACE", os);

		//��ֹ���ڿ���
		waitKey(1);
		
		//��40֡����
		this_thread::sleep_for(chrono::milliseconds(25));
	}
}

void Camera::ShowImage(Mat& image, bool& keep)
{
	imshow("image", image);
	while (waitKey() && keep);
}
