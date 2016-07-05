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
		m_currentKeyMap[keyID] = true; //Rather than using insert
	}

	void InputManager::releaseKey(unsigned int keyID)
	{
		m_currentKeyMap[keyID] = false;
	}

	bool InputManager::isKeyDown(unsigned int keyID)
	{
		auto it = m_currentKeyMap.find(keyID);
		if (it != m_currentKeyMap.end())
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

	bool InputManager::isKeyPressed(unsigned int keyID)
	{
		if (!wasKeyPressed(keyID) && isKeyDown(keyID))
		{
			return true;
		}

		return false;
	}

	void InputManager::update()
	{
		for (auto iter = m_currentKeyMap.begin(); iter != m_currentKeyMap.end(); iter++) //alternative for(auto& it : m_currentKeyMap) <-- C++ 11 Foreach
		{
			m_previousKeyMap[iter->first] = iter->second;
		}
	}

	bool InputManager::wasKeyPressed(unsigned int keyID)
	{
		if (m_previousKeyMap[keyID])
		{
			return true;
		}

		return false;
	}

}
