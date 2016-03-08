#include "ResourceManager.h"



TextureCache ResourceManager::_textureCache; //Declare static variable in cpp file necessary


GLTexture ResourceManager::getTexture(string textureFilePath) //Wrapper
{
	return _textureCache.getTexture(textureFilePath);
}
