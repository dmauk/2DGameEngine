#pragma once
#include<string>
#include<GL\glew.h>
using namespace std;

namespace GameEngine2D {

	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();
		void compileShaders(const string& vertexShaderFilePath, const string& fragmentShaderFilePath);
		void linkShaders();
		void addAttribute(const string& attributeName);
		GLint getUniformLocation(const string& uniformName);
		void use();
		void unuse();
	private:
		void compileShader(const string& filePath, GLuint shaderID);
		int m_numAttributes;
		GLuint m_programID;
		GLuint m_vertexShaderID;
		GLuint m_fragmentShaderID;
	};

}