#include "Sprite.h"
#include "Vertex.h"
#include "ErrorHandling.h"
#include <cstddef>
#include "ResourceManager.h"

namespace GameEngine2D {
	Sprite::Sprite()
	{
		_vboID = 0;
	}


	Sprite::~Sprite()
	{
		if (_vboID != 0) {
			glDeleteBuffers(1, &_vboID);
		}
	}

	void Sprite::init(float x, float y, float width, float height, string textureFilePath) {
		_x = x;
		_y = y;
		_width = width;
		_height = height;

		if (_vboID == 0) {
			glGenBuffers(1, &_vboID);
		}

		_texture = ResourceManager::getTexture(textureFilePath);

		Vertex vertexData[6];

		//First Triangle with UV
		//Upper right
		vertexData[0].setPosition(x + width, y + height); //Maybe convert this to a generic function that sets this as a generic quad. 
		vertexData[0].setUV(1.0f, 1.0f);

		//Upper left
		vertexData[1].setPosition(x, y + height);
		vertexData[1].setUV(0.0f, 1.0f);

		//Bottom left
		vertexData[2].setPosition(x, y);
		vertexData[2].setUV(0.0f, 0.0f);

		//Second Triangle with UV
		//Bottom left
		vertexData[3].setPosition(x, y);
		vertexData[3].setUV(0.0f, 0.0f);

		//Bottom right
		vertexData[4].setPosition(x + width, y);
		vertexData[4].setUV(1.0f, 0.0f);

		//Upper right
		vertexData[5].setPosition(x + width, y + height);
		vertexData[5].setUV(1.0f, 1.0f);


		for (int i = 0; i < 6; i++) //sizeof(vertexData)/sizeof(vertexData[0]) essentially 72/12 = 6
		{
			vertexData[i].setColor(255, 0, 255, 255);
		}

		vertexData[1].setColor(0, 0, 255, 255);

		vertexData[4].setColor(0, 255, 0, 255);


		//Bind buffer
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
		//Unbind buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	void Sprite::draw() {

		glBindTexture(GL_TEXTURE_2D, _texture.id); //Don't unbind this texture in the case another sprite will use this.

		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		//Point opengl to the data
		//Position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		//Color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		//UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//Unbind vbo
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}