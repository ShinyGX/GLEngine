#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>

using std::ifstream;
using std::exception;
using std::stringstream;

#define TYPE_VERTEX_SHADER        "VERTEX_SHADER"
#define TYPE_FRAGMENT_SHADER      "FRAGMENT_SHADER"
#define TYPE_SHADER_PROGRAM       "SHADER_PROGRAM"

unsigned int Shader::createShaderProgram() const
{
	const auto vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexShaderCode, nullptr);
	glCompileShader(vertex);
	checkCompileErrors(vertex, TYPE_VERTEX_SHADER);

	const auto fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentShaderCode, nullptr);
	glCompileShader(fragment);
	checkCompileErrors(vertex, TYPE_FRAGMENT_SHADER);

	const auto shaderProgramId = glCreateProgram();
	glAttachShader(shaderProgramId, vertex);
	glAttachShader(shaderProgramId, fragment);
	glLinkProgram(shaderProgramId);
	checkCompileErrors(shaderProgramId, TYPE_SHADER_PROGRAM);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return shaderProgramId;
}

void Shader::checkCompileErrors(unsigned int id, const std::string& type)
{
	int success;
	char logInfo[512];

	if (type != TYPE_SHADER_PROGRAM)
	{
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, 512, nullptr, logInfo);
			throw exception(logInfo);
		}
	}
	else
	{
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, 512, nullptr, logInfo);
			throw exception(logInfo);
		}
	}


}

Shader::Shader(const char * vertexPath, const char * fragmentPath)
{
	ifstream vertexFile;
	ifstream fragmentFile;

	vertexFile.exceptions(ifstream::failbit | ifstream::badbit);
	fragmentFile.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);

		if (!vertexFile.is_open() || !fragmentFile.is_open())
			throw exception("open file failed");

		stringstream vertexSStream;
		stringstream fragmentSStream;

		vertexSStream << vertexFile.rdbuf();
		fragmentSStream << fragmentFile.rdbuf();

		auto vertexString = vertexSStream.str();
		auto fragmentString = fragmentSStream.str();

		vertexShaderCode = vertexString.c_str();
		fragmentShaderCode = fragmentString.c_str();

		id = createShaderProgram();

		std::cout << vertexShaderCode << std::endl;
		std::cout << fragmentShaderCode << std::endl;
	}
	catch (const exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void Shader::use() const
{
	glUseProgram(id);
}

void Shader::setBool(const std::string & name, const bool value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(value));
}

void Shader::setFloat(const std::string & name, const float value) const
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setInt(const std::string & name, const int value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setUnsignInt(const std::string & name, const unsigned int value) const
{
	glUniform1ui(glGetUniformLocation(id, name.c_str()), value);
}
