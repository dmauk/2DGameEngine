#include "InputManager.h"
#include <iostream>

using namespace std;

namespace GameEngine2D {

	InputManager::InputManager()
	{
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::pressKey(unsigned int keyID)
	{
		m_keyMap[keyID] = true; //Rather than using insert
	}

	void InputManager::releaseKey(unsigned int keyID)
	{
		m_keyMap[keyID] = false;
	}

	bool InputManager::isKeyPressed(unsigned int keyID)
	{
		auto it = m_keyMap.find(keyID);
		if (it != m_keyMap.end())
		{
			return it->second; //Alternatively return true; Wouldn't necessarily work however because you could find a key that was released and say it is pressed.
		}
		else
		{
			return false;
		}
	}

	void InputManager::setMouseCoords(float x, float y)
	{
		m_mouseCoords.x = x;
		m_mouseCoords.y = y;
	}

	glm::vec2 InputManager::getMouseCoords()
	{
		return m_mouseCoords;
	}

}
