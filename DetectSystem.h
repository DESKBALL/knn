#pragma once

#include "FaceEngine.h"

enum menu { QUIT, DETECT, NEWFACE };

extern FaceEngine	faceThread;
extern Camera		cameraThread;

void DetectSystem();
void SaveNewFace();