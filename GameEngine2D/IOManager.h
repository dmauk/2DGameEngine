#pragma once
#include<vector>
#include<string>
using namespace std;

namespace GameEngine2D {
	//Static class (cannot be instanced)
	class IOManager
	{
	public:
		static bool readFileToBuffer(const string filePath, vector<unsigned char>& buffer);
	};

}