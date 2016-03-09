#include"ErrorHandling.h"
#include<iostream>
#include<SDL\SDL.h>
#include<cstdlib>

namespace GameEngine2D {

	void fatalError(string errorString) {
		cout << errorString << endl;
		cout << "Enter any key to quit";
		int tmp;
		cin >> tmp;
		SDL_Quit();
		exit(1);
	}
}