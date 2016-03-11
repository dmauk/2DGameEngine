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
		void setPosition(glm::vec2& newPosition) { _position = newPosition; _needMatrixUpdate = true; }
		void setScale(float newScale) { _scale = newScale; _needMatrixUpdate = true; }

		//Getters
		glm::vec2 getPosition() { return _position; }
		float getScale() { return _scale; }
		glm::mat4 getCameraMatrix() { return _cameraMatrix; }

		glm::vec2 convertScreenToWorldCoords(glm::vec2 screenCoords);

	private:
		int _screenWidth;
		int _screenHeight;
		bool _needMatrixUpdate;
		float _scale;
		glm::vec2 _position;
		glm::mat4 _cameraMatrix;
		glm::mat4 _orthoMatrix;
	};

}