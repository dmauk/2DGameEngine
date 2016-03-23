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

		void pressKey(unsigned int keyID); //Wrapping in own key binding enum would be good for future 23:3:44
		void releaseKey(unsigned int keyID);
		void update();
		bool isKeyPressed(unsigned int keyID);
		bool isKeyDown(unsigned int keyID);
		void setMouseCoords(float x, float y);
		glm::vec2 getMouseCoords();
	private:
		bool wasKeyPressed(unsigned int keyID);
		unordered_map<unsigned int, bool> m_currentKeyMap;
		unordered_map<unsigned int, bool> m_previousKeyMap;
		glm::vec2 m_mouseCoords;
	};
}

