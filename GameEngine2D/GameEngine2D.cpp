#pragma once
#include<SDL\SDL.h>
#include<GL\glew.h>
#include "GameEngine2D.h"

namespace GameEngine2D {
	int GameEngine2D::init()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //Set state before creating window
		return 0;
	}
}

