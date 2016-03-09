#pragma once
#include<map>
#include<string>
#include"GLTexture.h"

using namespace std;

namespace GameEngine2D {
	//Add model cache, sound cache, etc.
	class TextureCache
	{
	public:
		TextureCache();
		~TextureCache();

		GLTexture getTexture(string textureFilePath); //Can be improved by returning a pointer rather than make a copy.
	private:
		map<string, GLTexture> _textureMap;
	};

}
