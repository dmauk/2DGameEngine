#include "ResourceManager.h"


namespace GameEngine2D {
	TextureCache ResourceManager::m_textureCache; //Declare static variable in cpp file necessary


	GLTexture ResourceManager::getTexture(string textureFilePath) //Wrapper
	{
		return m_textureCache.getTexture(textureFilePath);
	}

}