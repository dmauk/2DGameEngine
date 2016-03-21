#include "Camera2D.h"

namespace GameEngine2D {

	Camera2D::Camera2D() : m_position(1.0f, 1.0f), m_cameraMatrix(1.0f), m_orthoMatrix(1.0f), m_scale(1.0f), m_needMatrixUpdate(true) , m_screenWidth(1024), m_screenHeight(768)
	{

	}


	Camera2D::~Camera2D()
	{
	}

	void Camera2D::init(int screenWidth, int screenHeight)
	{
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;
		m_orthoMatrix = glm::ortho(0.0f, (float)m_screenWidth, 0.0f, (float)m_screenHeight);
	}

	void Camera2D::update()
	{
		if (m_needMatrixUpdate)
		{
			glm::vec3 translate(-m_position.x + m_screenWidth / 2, -m_position.y + m_screenHeight / 2, 0.0f); //This does matrix math for 3dimensions when you only need two.
			m_cameraMatrix = glm::translate(m_orthoMatrix, translate);
			glm::vec3 scale(m_scale, m_scale, 0.0f); //0.5f would zoom out, 2.0f would zoom in.
			m_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * m_cameraMatrix; //Alternative (scale,_scale) * _cameraMatrix;
			m_needMatrixUpdate = false;
		}
	}

	glm::vec2 Camera2D::convertScreenToWorldCoords(glm::vec2 screenCoords)
	{
		//Invert Y direction
		screenCoords.y = m_screenHeight - screenCoords.y;
		//Make it so 0,0 is the center of the screen
		screenCoords -= glm::vec2(m_screenWidth / 2, m_screenHeight / 2);
		//Scale the coordinates
		screenCoords /= m_scale;
		//Translate with the camera position
		screenCoords += m_position;

		return screenCoords;
	}
	
}
