#pragma once
#include<GLM/glm.hpp>
#include<GLM/gtc/matrix_transform.hpp> //Used to build orthographic matrix

namespace GameEngine2D {

	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		void init(int screenWidth, int screenHeight);
		void update();

		//Setters
		void setPosition(glm::vec2& newPosition) { m_position = newPosition; m_needMatrixUpdate = true; }
		void setScale(float newScale) { m_scale = newScale; m_needMatrixUpdate = true; }

		//Getters
		glm::vec2 getPosition() { return m_position; }
		float getScale() { return m_scale; }
		glm::mat4 getCameraMatrix() { return m_cameraMatrix; }

		glm::vec2 convertScreenToWorldCoords(glm::vec2 screenCoords);

	private:
		int m_screenWidth;
		int m_screenHeight;
		bool m_needMatrixUpdate;
		float m_scale;
		glm::vec2 m_position;
		glm::mat4 m_cameraMatrix;
		glm::mat4 m_orthoMatrix;
	};

}