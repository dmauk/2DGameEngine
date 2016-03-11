#pragma once
#include<unordered_map>
#include<GLM\glm.hpp>

using namespace std;
namespace GameEngine2D {

	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void pressKey(unsigned int keyID); //Wrapping in own key binding enum would be good for futre 23:3:44
		void releaseKey(unsigned int keyID);
		bool isKeyPressed(unsigned int keyID);
		void setMouseCoords(float x, float y);
		glm::vec2 getMouseCoords();
	private:
		unordered_map<unsigned int, bool> _keyMap;
		glm::vec2 _mouseCoords;
	};
}

