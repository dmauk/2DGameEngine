#pragma once
#include<GL\glew.h>
#include"GLTexture.h"
#include<string>


using namespace std;

namespace GameEngine2D {

	class Sprite
	{
	public:
		Sprite();
		~Sprite();
		void init(float x, float y, float width, float height, string textureFilePath);
		void draw();

	private:

		float _x;
		float _y;
		float _width;
		float _height;
		GLuint _vboID;
		GLTexture _texture; //look up ref loading for textures
	};

}