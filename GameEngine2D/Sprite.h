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

		float m_x;
		float m_y;
		float m_width;
		float m_height;
		GLuint m_vboID;
		GLTexture m_texture; //look up ref loading for textures
	};

}