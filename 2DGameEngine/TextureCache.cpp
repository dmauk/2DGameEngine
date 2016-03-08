#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>

TextureCache::TextureCache()
{
}


TextureCache::~TextureCache()
{
}

GLTexture TextureCache::getTexture(string textureFilePath)
{
	map<string,GLTexture>::iterator mapIterator = _textureMap.find(textureFilePath); //Check if the texture is in the cache //returns an iterator to the texture
	//auto mapIteratator = _textureMap.find(textureKey); good for long template types 

	//Check if not in map
	if (mapIterator == _textureMap.end()) {
		//If not in cache,
		//Load texture
		GLTexture newTexture = ImageLoader::loadPNG(textureFilePath);
		//std::pair<string, GLTexture> newPair(textureFilePath,newTexture);
		//_textureMap.insert(newPair);

		//Insert it into the map
		_textureMap.insert(make_pair(textureFilePath, newTexture));

		cout << "Loaded new texture" << endl;

		return newTexture;
	}
	cout << "Loaded new texture" << endl;
		//If in cache,
	return mapIterator->second; 

}