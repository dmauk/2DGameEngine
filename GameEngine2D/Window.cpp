#include "Window.h"
#include <iostream>
#include "ErrorHandling.h"


namespace GameEngine2D {

	Window::Window()
	{
	}


	Window::~Window()
	{
	}

	int Window::create(string windowName, int screenWidth, int screenHeight, unsigned int currentFlags)
	{
		Uint32 flags = SDL_WINDOW_OPENGL;

		if (currentFlags & INVISIBLE)
		{
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & FULLSCREEN)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlags & BORDERLESS)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		if (_sdlWindow == nullptr) {
			fatalError("SDL Window could not be created!");
		}
		SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
		if (glContext == nullptr) {
			fatalError("SDL_GL context could not be created!");
		}
		GLenum error = glewInit();
		if (error != GLEW_OK) {
			fatalError("Could not initialize glew");
		};


		//Display OPENGL VERSION FOR USER
		cout << "OPENGL VERSION: " << glGetString(GL_VERSION) << endl; //GL_VERSION MACRO IS SET TO WHATEVER OPENGL VERSION YOU ARE RUNNING
		glClearColor(0.0f,0.0f,1.0f,1.0f);


		SDL_GL_SetSwapInterval(1); //Enable/Disables V-Sync
		
		//Enable blending based on alpha
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); //take source alpha and multiply by color of texture at all points, take background color and be transparent to the src alpha minus one 
														  //IE: in places where transparency is 0 on imager we get 1 for the background show-through, or whatever decimal it needs.
		return 0;
	}

	void Window::swapBuffer()
	{
		SDL_GL_SwapWindow(_sdlWindow);
	}

	int Window::getScreenWidth()
	{
		return _screenWidth;
	}

	int Window::getScreenHeight()
	{
		return _screenHeight;
	}

}