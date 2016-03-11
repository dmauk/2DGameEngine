#include "Camera2D.h"

namespace GameEngine2D {

	Camera2D::Camera2D() : _position(1.0f, 1.0f), _cameraMatrix(1.0f), _orthoMatrix(1.0f), _scale(1.0f), _needMatrixUpdate(true) , _screenWidth(1024), _screenHeight(768)
	{

	}


	Camera2D::~Camera2D()
	{
	}

	void Camera2D::init(int screenWidth, int screenHeight)
	{
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
	}

	void Camera2D::update()
	{
		if (_needMatrixUpdate)
		{
			glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0.0f); //This does matrix math for 3dimensions when you only need two.
			_cameraMatrix = glm::translate(_orthoMatrix, translate);
			glm::vec3 scale(_scale, _scale, 0.0f); //0.5f would zoom out, 2.0f would zoom in.
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix; //Alternative (scale,_scale) * _cameraMatrix;
			_needMatrixUpdate = false;
		}
	}

	glm::vec2 Camera2D::convertScreenToWorldCoords(glm::vec2 screenCoords)
	{
		//Invert Y direction
		screenCoords.y = _screenHeight - screenCoords.y;
		//Make it so 0,0 is the center of the screen
		screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);
		//Scale the coordinates
		screenCoords /= _scale;
		//Translate with the camera position
		screenCoords += _position;

		return screenCoords;
	}
	
}
