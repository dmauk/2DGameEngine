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

	bool Camera2D::isBoxInView(const glm::vec2& position, const glm::vec2& dimensions)
	{
		glm::vec2 scaledScreenDimensions = glm::vec2(m_screenWidth, m_screenHeight) / m_scale;
		//Minimum distance before a colision will occur
		const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaledScreenDimensions.x / 2.0f;
		const float MIN_DISTANCE_Y = dimensions.y / 2.0f + scaledScreenDimensions.y / 2.0f;

		//Center positions
		glm::vec2 centerPosition = position + (dimensions / 2.0f);
		glm::vec2 centerCameraPosition = m_position;

		//Vector from the input to the camera
		glm::vec2 distVec = centerPosition - centerCameraPosition;

		//Get the depth of the collision
		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		if (xDepth > 0 && yDepth > 0) //Collision
		{
			//There was a collision
			return true;
		}
		return false;
	}
	
}
