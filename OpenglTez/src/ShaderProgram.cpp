#include"ShaderProgram.hpp"
#include<glad/glad.h>
#include<iostream>
#include<fstream>

ShaderProgram::ShaderProgram()
{
	this->_programId = glCreateProgram();

}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(this->_programId);
}

void ShaderProgram::AttachShader(const char* filename, unsigned int shaderType)
{
	unsigned int shaderId = glCreateShader(shaderType);
	std::string shaderData = GetShaderFromFile(filename);
	const char* shaderP = &shaderData[0];
	glShaderSource(shaderId, 1, &shaderP, nullptr);
	glCompileShader(shaderId);

	// catch error
	int success;
	char log[512];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderId, 512, NULL, log);
		std::cout << "Error at shader compile - file name : "<< filename << std::endl << log << std::endl;
	}

	glAttachShader(this->_programId, shaderId);
	glDeleteShader(shaderId);
}

void ShaderProgram::AddUniformVariable(const std::string& varName)
{
	this->_uniformTable[varName] = glGetUniformLocation(this->_programId, varName.c_str());
}

unsigned int ShaderProgram::GetUniformId(const std::string& varName)
{
	if (this->_uniformTable.count(varName) == 0)
	{
		AddUniformVariable(varName);
	}
	return this->_uniformTable[varName];
}

unsigned int ShaderProgram::GetProgramId()
{
	return _programId;
}

void ShaderProgram::SetMat4(const std::string& varName, const glm::mat4& var)
{
	glUniformMatrix4fv(GetUniformId(varName), 1, GL_FALSE, (GLfloat*)& var);
}

void ShaderProgram::SetVec3(const std::string& varName, const glm::vec3& var)
{
	glUniform3f(GetUniformId(varName), var.x, var.y, var.z);
}

void ShaderProgram::SetInt(const std::string& varName,const int& var)
{
	glUniform1i(GetUniformId(varName), var);
}

void ShaderProgram::SetFloat(const std::string& varName, const float& var)
{
	glUniform1f(GetUniformId(varName), var);
}

void ShaderProgram::Link()
{
	glLinkProgram(this->_programId);

	int isLinked = 1;
	char log[512];
	glGetProgramiv(this->_programId, GL_COMPILE_STATUS, &isLinked);

	if (!isLinked)
	{
		glGetProgramInfoLog(this->_programId, 512, 0, log);

		std::cout << "Program Linking Error !! - > " << log << std::endl;
	}
}

void ShaderProgram::Use()
{
	glUseProgram(this->_programId);
}

std::string ShaderProgram::GetShaderFromFile(const char* filename)
{
	std::ifstream file(filename);
	std::string data;

	if (file.is_open())
	{
		char readChar;
		while ((readChar = file.get()) != EOF)
		{
			data += readChar;
		}
		file.close();
	}
	return data;
}
