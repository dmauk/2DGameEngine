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
		_keyMap[keyID] = true; //Rather than using insert
	}

	void InputManager::releaseKey(unsigned int keyID)
	{
		_keyMap[keyID] = false;
	}

	bool InputManager::isKeyPressed(unsigned int keyID)
	{
		auto it = _keyMap.find(keyID);
		if (it != _keyMap.end())
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
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}

	glm::vec2 InputManager::getMouseCoords()
	{
		return _mouseCoords;
	}

}
