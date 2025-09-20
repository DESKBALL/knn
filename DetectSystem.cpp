
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
	cout << "==��ѡ��ģʽ: 1.����ʶ��  0.�˳�          =====\n";
	cout << "===============================================\n";
	cout << '\n';
	cout << "������: ";
}

void DetectThread()
{
	run = true;
	thread camera(&Camera::RunCamera, &cameraThread, ref(run));
	thread face(&FaceEngine::EngineThread, &faceThread, ref(run), ref(cameraThread));
	this_thread::sleep_for(chrono::seconds(5));

	char ch;
	system("cls");
	cout << "���������У����� Q �˳�\n:";
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
	
	char type;	//ʹ���ַ��ͷ�ֹ�������ַ��ǳ���
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
		default:cout << "����δ֪����!\n"; system("pause"); break;
		}
	}

	

	cin.get();
	
	
}