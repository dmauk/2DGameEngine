#pragma once

#include <GL/glew.h>

namespace GameEngine2D {
	struct Position {
		float x;
		float y;
	};

	struct ColorRGBA8 {
		ColorRGBA8() : r(0),g(0),b(0),a(0) {}
		ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A) : r(R), g(G), b(B), a(A) {}
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};

	struct UV {
		float u;
		float v;
	};

	struct Vertex {
		//Composition because a struct is defined within a struct.
		//Laid out exact same way in memory as using float position[2]
		Position position; //Same as calling Position position
		ColorRGBA8 color;
		//float position[2]; //2 * 4 bytes
		//GLubyte color[4]; //4 * 1 byte for a nice 12 byte (multiple of 4)
		UV uv;


		void setPosition(float x, float y)
		{
			position.x = x;
			position.y = y;
		}

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
		{
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}
		void setUV(float u, float v)
		{
			uv.u = u;
			uv.v = v;
		}
	};
}