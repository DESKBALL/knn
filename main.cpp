
#include <iostream>
#include <fstream>
#include <thread>
#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <arcsoft_face_sdk.h> 

#include "DetectSystem.h"

using namespace std;
using namespace cv;

const MPChar APP_ID = (MPChar)"4UVZzzZSKsWwEBMuKrBFyfqhYVmAUGFwFDmUik9kDJDN";
const MPChar SDK_KEY = (MPChar)"CVZRULqDSFRRLpd3nxW7TYH2Zjmxayd4WAzYS8Yb3vHP";

int main()
{
	MRESULT res = ASFOnlineActivation(APP_ID, SDK_KEY);
	if (res != MOK && res != MERR_ASF_ALREADY_ACTIVATED) {
		printf("Error %x: failed to Activate SDK!\n", res);
		return res;
	}
	DetectSystem();
	
	return 0;
}