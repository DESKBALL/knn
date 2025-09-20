
#include <iostream>
#include <mutex>

#include "DetectSystem.h"

using namespace std;

FaceEngine faceThread;
Camera	cameraThread;
bool run;

void ShowMenu()
{
	cout << "===============================================\n";
	cout << "==请选择模式: 1.人脸识别  0.退出          =====\n";
	cout << "===============================================\n";
	cout << '\n';
	cout << "请输入: ";
}

void DetectThread()
{
	run = true;
	thread camera(&Camera::RunCamera, &cameraThread, ref(run));
	thread face(&FaceEngine::EngineThread, &faceThread, ref(run), ref(cameraThread));
	this_thread::sleep_for(chrono::seconds(5));

	char ch;
	system("cls");
	cout << "程序运行中，输入 Q 退出\n:";
	while (true)
	{
		cin.get(ch);
		if ((ch | 'q') == 'q') {
			break;
		}
	}

	run = false;
	camera.join();
	face.join();
}

void DetectSystem()
{
	
	char type;	//使用字符型防止误输入字符是出错
	bool runMenu = true;

	while (runMenu)
	{
		system("cls");
		ShowMenu();
		cin >> type;

		switch (type - '0')
		{
		case DETECT: DetectThread(); break;
		//case NEWFACE: SaveNewFace(); break;
		case QUIT: runMenu = false; break;
		default:cout << "错误！未知输入!\n"; system("pause"); break;
		}
	}

	

	cin.get();
	
	
}