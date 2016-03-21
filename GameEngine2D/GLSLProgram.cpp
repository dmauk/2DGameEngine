
#include "GLSLProgram.h"
#include"ErrorHandling.h"
#include<fstream>
#include<vector>
using namespace std;
namespace GameEngine2D {
	GLSLProgram::GLSLProgram() : m_numAttributes(0), m_programID(0), m_vertexShaderID(0), m_fragmentShaderID(0)
	{

	}


	GLSLProgram::~GLSLProgram()
	{
	}

	void GLSLProgram::compileShaders(const string& vertexShaderFilePath, const string& fragmentShaderFilePath)
	{
		m_programID = glCreateProgram();
		//Create Vertex Shader
		m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (m_vertexShaderID == 0) {
			fatalError("Vertex Shader Failed to be created.");
		}

		//Create Fragment Shader
		m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (m_fragmentShaderID == 0) {
			fatalError("Fragment Shader Failed to be created.");
		}

		compileShader(vertexShaderFilePath, m_vertexShaderID);

		compileShader(fragmentShaderFilePath, m_fragmentShaderID);
	}

	void GLSLProgram::compileShader(const string& filePath, GLuint shaderID)
	{
		ifstream vertexFile(filePath);
		if (vertexFile.fail()) {
			perror(filePath.c_str());
			fatalError("Failed to open: " + filePath);
		}

		string fileContents = "";
		string line;
		while (getline(vertexFile, line))
		{
			fileContents += line + "\n";
		}

		vertexFile.close();
		const char* contentsPtr = fileContents.c_str();
		glShaderSource(shaderID, 1, &contentsPtr, nullptr);
		glCompileShader(shaderID);

		//Error Checking
		GLint success = 0;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			vector<char> errorLog(maxLength);
			glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

			//Provide the infolog in whatever manor you demem best.
			//Exit with failure.
			glDeleteShader(shaderID);//Don't leak shader.
			printf("%s\n", &(errorLog[0]));//Passing reference of vector's first element causes it to be treated as standard string.
			fatalError("Shader " + filePath + " failed to compile");
			return;
		}
	}

	void GLSLProgram::linkShaders()
	{
		//Vertex and fragment shaders are successfully compiled.
		//Time to link them together into a program
		//Attach shaders to program
		glAttachShader(m_programID, m_vertexShaderID);
		glAttachShader(m_programID, m_fragmentShaderID);

		//Link our program
		glLinkProgram(m_programID);

		//Note that we use glGetProgram instead of glGetShader
		GLint isLinked = 0;
		glGetProgramiv(m_programID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULl character
			vector<char> errorLog(maxLength);
			glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);

			//We don't need the program anymore.
			glDeleteProgram(m_programID);
			//Don't leak shaders
			glDeleteShader(m_vertexShaderID);
			glDeleteShader(m_fragmentShaderID);


			//Use the infolog as you see fit.
			printf("%s\n", &(errorLog[0]));//Passing reference of vector's first element causes it to be treated as standard string.
			fatalError("Failed to link shaders");


			return;
		}
		glDetachShader(m_programID, m_vertexShaderID);
		glDetachShader(m_programID, m_fragmentShaderID);
		glDeleteShader(m_vertexShaderID);
		glDeleteShader(m_fragmentShaderID);
	}

	void GLSLProgram::addAttribute(const string& attributeName)
	{
		//There is a modern version of this function that can be used!
		glBindAttribLocation(m_programID, m_numAttributes++, attributeName.c_str()); //Bind attribute and iterate by 1
	}


	GLint GLSLProgram::getUniformLocation(const string& uniformName)
	{
		GLint location = glGetUniformLocation(m_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX)
		{
			fatalError("Uniform " + uniformName + " not found in shader!");
		}

		return location;
	}

	void GLSLProgram::use()
	{
		glUseProgram(m_programID);
		for (int i = 0; i < m_numAttributes; i++)
		{
			glEnableVertexAttribArray(i);
		}
	}

	void GLSLProgram::unuse()
	{
		glUseProgram(0);
		for (int i = 0; i < m_numAttributes; i++)
		{
			glDisableVertexAttribArray(i);
		}
	}

}


