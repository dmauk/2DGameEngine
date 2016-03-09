#pragma once
#include "TextureCache.h"
#include<string>
using namespace std;
//Static class //LOOK UP SINGLETON

namespace GameEngine2D {

	class ResourceManager
	{
	public:
		static GLTexture getTexture(string textureFilePath);
		//Have to make sure that a texture is not being rebound multiple times.
	private:
		static TextureCache _textureCache;
	};

}

