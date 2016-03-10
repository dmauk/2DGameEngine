#pragma once
#include<unordered_map>

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
	private:
		unordered_map<unsigned int, bool> _keyMap;
	};
}

