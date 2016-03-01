#include<iostream>
#include"MainGame.h"
using namespace std;

int main(int argc, char** argv)
{
	MainGame mainGame;
	mainGame.run();
	cout << "Enter any key to quit..." << endl;
	int a;
	cin >> a;
	return 0;
}