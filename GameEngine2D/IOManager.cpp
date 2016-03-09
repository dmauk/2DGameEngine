#include "IOManager.h"

#include<fstream>
using namespace std;
namespace GameEngine2D {
	bool IOManager::readFileToBuffer(string filePath, vector<unsigned char>& buffer)
	{
		ifstream file(filePath, ios::binary);
		if (file.fail())
		{
			perror(filePath.c_str());
			return false;
		}

		//seek to the end of the file
		file.seekg(0, ios::end);

		//Get file size
		int fileSize = file.tellg();

		//seek to begin
		file.seekg(0, ios::beg);

		//Reduce the file size by any header bytes that may be present
		fileSize -= file.tellg();

		buffer.resize(fileSize);
		file.read((char*)&(buffer[0]), fileSize);//Read everything as if they were signed chars
		file.close();

		return true;

	}
}