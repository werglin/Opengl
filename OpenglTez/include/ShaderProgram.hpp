#pragma once

#ifndef SHADERPROGRAM_hpp
#define SHADERPROGRAM_hpp

#include<string>
#include<map>
#include<glm/glm.hpp>

class ShaderProgram {
public:
	ShaderProgram();
	~ShaderProgram();
	void AttachShader(const char* filename, unsigned int shaderType);
	unsigned int GetUniformId(const std::string& varName);
	unsigned int GetProgramId();
	void SetMat4(const std::string& varName, const glm::mat4& var);
	void SetVec3(const std::string& varName, const glm::vec3& var);
	void SetInt(const std::string& varName, const int& var);
	void SetFloat(const std::string& varName, const float& var);
	void Link();
	void Use();

private:
	std::string GetShaderFromFile(const char* filename);
	void AddUniformVariable(const std::string& varName);

	std::map<std::string, unsigned int> _uniformTable;
	unsigned int _programId;

};


#endif