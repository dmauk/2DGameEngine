#include "Timing.h"

#include <SDL\SDL.h>

namespace GameEngine2D {

	FpsLimiter::FpsLimiter()
	{

	}
	void FpsLimiter::init(float maxFPS)
	{
		setMaxFPS(maxFPS);
	}

	void FpsLimiter::setMaxFPS(float maxFPS)
	{
		m_maxFPS = maxFPS;
	}

	void FpsLimiter::begin()
	{
		m_startTicks = SDL_GetTicks();
	}
	float FpsLimiter::end()
	{
		calculateFPS();
		//Limit FPS //Already handled by vsync however
		float frameTicks = SDL_GetTicks() - m_startTicks;
		if (1000.0f / m_maxFPS > frameTicks)
		{
			SDL_Delay(1000.0f / m_maxFPS - frameTicks); // 1s / 60 = every 0.0166 second we have a frame. So every 1000*0.0166 = 16.66ms we have one frame. 
		}											   // if we have more frames in a shorter ms interval then we want to delay to the standard 60s interval. 

		return m_fps;
	}

	void FpsLimiter::calculateFPS()
	{
		static const int NUM_SAMPLES = 10; //Used to average last 10 frames for smooth frame rate measurment
		static float frameTimes[NUM_SAMPLES];
		static int currentBufferLocation;
		static float prevTicks = SDL_GetTicks();
		float currentTicks;
		currentTicks = SDL_GetTicks();
		m_frameTime = currentTicks - prevTicks; //Will be incorrect on the first reading because it will be 0
		frameTimes[currentBufferLocation%NUM_SAMPLES] = m_frameTime; //Use modulus to do a circular buffer.

		prevTicks = currentTicks;//Update previous ticks.
		int count;
		currentBufferLocation++; //Avoid divide by zero by iterating here
		if (currentBufferLocation < NUM_SAMPLES)//In the first few moments, the averages will based off numbers 1- 10 rather than 10 for accuracy.
		{
			count = currentBufferLocation;
		}
		else
		{
			count = NUM_SAMPLES;//Eventually everything will be averaged by NUM_SAMPLES or 10
		}

		float frameAverage = 0;
		for (int i = 0; i < count; i++)
		{
			frameAverage += frameTimes[i];
		}
		frameAverage /= count;

		if (frameAverage > 0)
		{
			m_fps = 1000.0f / frameAverage; //frameAverage = FPMS -> How many frames is that a second? 1 second = 1000 ms so 1000 / FPMS = FPS
		}
		else
		{
			m_fps = 60.0f; //This will only occur in the first case when the frame calculation will divide by zero
		}
	}

}