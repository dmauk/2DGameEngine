#pragma once

//Will hold on timing classes and functions

namespace GameEngine2D {
	class FpsLimiter {
	public:
		FpsLimiter();
		void init(float maxFPS);
		void setMaxFPS(float maxFPS);
		void begin();
		float end(); //Will return fps
	private:
		void calculateFPS();
		float _maxFPS;
		unsigned int _startTicks; //unsigned int is not guaranteed to be 4 bytes.
		float _frameTime;
		float _fps;
	};
}