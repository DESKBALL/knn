
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

		//检查缓存队列
		if (mtx.try_lock()) {
			Scalar color;
			for (size_t i = 0; i < RectBuffer.size(); i++)
			{
				//未知为红色，已知为绿色
				if (RectBuffer[i].info == "Unknown") {
					color = Scalar(0x14, 0x32, 0xff);
				}
				else {
					color = Scalar(0x32, 0xff, 0x14);
				}

				//画框
				rectangle(os, RectBuffer[i].rect, color, 2);
				//绘制关联信息
				Point textPoint(RectBuffer[i].rect.x,
					RectBuffer[i].rect.y + RectBuffer[i].rect.height + 20);
				putText(os, RectBuffer[i].info, textPoint, FONT_HERSHEY_DUPLEX, 0.8, color);
			}
			//RectBuffer.clear();
			mtx.unlock();
		}

		imshow("DETECTFACE", os);

		//防止窗口卡死
		waitKey(1);
		
		//以40帧运行
		this_thread::sleep_for(chrono::milliseconds(25));
	}
}

void Camera::ShowImage(Mat& image, bool& keep)
{
	imshow("image", image);
	while (waitKey() && keep);
}
