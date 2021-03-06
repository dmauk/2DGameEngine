#pragma once
#include"GLTexture.h"
#include<string>
using namespace std;

namespace GameEngine2D {

	//Static Class (cannot be instanced)
	class ImageLoader
	{
	public:
		static GLTexture loadPNG(const string filePath);
	};

}