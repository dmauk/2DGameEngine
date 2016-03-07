#pragma once
#include<vector>
#include<string>
using namespace std;
//Static class (cannot be instanced)
class IOManager
{
public:
	static bool readFileToBuffer(const string filePath,vector<unsigned char>& buffer);
};

