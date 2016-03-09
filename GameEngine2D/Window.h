#pragma once
#include<SDL/SDL.h>
#include<GL\glew.h>
#include<string>
using namespace std;

namespace GameEngine2D {
	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 }; //Hex values

	class Window
	{
	public:
		Window();
		~Window();

		int create(string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);
		int getScreenWidth();
		int getScreenHeight(); //Alternative int getScreenHeight() { _screenHeight; }
		void swapBuffer();

	private:
		SDL_Window* _sdlWindow;
		int _screenWidth;
		int _screenHeight;
	};

}