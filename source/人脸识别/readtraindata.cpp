
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

using namespace std;

#include "detectface.h"


int readtraindata(string path)
{
	size_t type = 0;

	for (const auto& entry : filesystem::directory_iterator(path))
	{
		//搜寻路径中.dat后缀文件
		if (entry.path().extension() == ".dat")
		{
			//打开文件
			ifstream in_file(entry.path(), ios::binary);
			if (!in_file.is_open()) {
				printf("Error: failed to opened traindata.\n");
				return -1;
			}

			//获取文件名中的信息
			string this_face=entry.path().filename().string();
			this_face.erase(this_face.size() - 4, 4);
			_file_name_.push_back(this_face);

			//读取数据
			int sizeNums = 0;			// 样本字节数
			float* feature = NULL;		// 样本

			while (in_file.read((char*)&sizeNums, sizeof(MInt32)))
			{
				//申请堆储存样本
				feature = (float*)malloc(sizeNums);
				
				//读取样本
				in_file.read((char*)feature, sizeNums);

				//存入数据向量中
				FACEPOINT buffer;
				buffer.feature = feature;
				buffer.type = type;
				_train_data_.push_back(buffer);
			}
			
			type++;
			in_file.close();
		}
	}

	return type;
}