#include "ImageLoader.h"
#include "picoPNG.h"
#include "IOManager.h"
#include "ErrorHandling.h"

namespace GameEngine2D {


	GLTexture ImageLoader::loadPNG(const string filePath)
	{
		GLTexture texture = {};
		vector<unsigned char> in; //possible optimization here
		vector<unsigned char> out;
		unsigned long width, height;
		if (IOManager::readFileToBuffer(filePath, in) == false)
		{
			fatalError("Failed to load PNG file to buffer!");
		}
		int errorCode = decodePNG(out, width, height, &(in[0]), in.size()); //convert to RGBA is already true so no specification needed.
		if (errorCode != 0) {
			fatalError("decodePNG failed with error: " + to_string(errorCode));
		}

		glGenTextures(1, &(texture.id)); //Generated texture and set texture id to that texture
		glBindTexture(GL_TEXTURE_2D, texture.id); //Bind texture 2D to texture id
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0])); //Can't just use &(out) because there is header memory for the texture that you will end up using
		//How to treat texture?
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //Mipmapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0); //Unbind the current texture
		texture.width = width;
		texture.height = height;
		return texture;
	}

}


