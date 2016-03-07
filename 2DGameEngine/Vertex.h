#pragma once

#include <GL/glew.h>
struct Position {
	float x;
	float y;
};

struct Color {
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
};

struct Vertex {
	//Composition because a struct is defined within a struct.
	//Laid out exact same way in memory as using float position[2]
	Position position; //Same as calling Position position
	Color color;
	//float position[2]; //2 * 4 bytes
	//GLubyte color[4]; //4 * 1 byte for a nice 12 byte (multiple of 4)
};