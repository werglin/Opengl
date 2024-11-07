#pragma once

#ifndef SHADERPROGRAM_hpp
#define SHADERPROGRAM_hpp

#include<string>
#include<map>
#include<glm/glm.hpp>
#include<assimp/scene.h>

class ShaderProgram {
public:
	ShaderProgram();
	~ShaderProgram();
	void AttachShader(const char* filename, unsigned int shaderType);
	unsigned int GetUniformId(const std::string& varName);
	unsigned int GetProgramId();
	void SetMat4(const std::string& varName, const glm::mat4& var);
	void SetVec3(const std::string& varName, const glm::vec3& var);
	void SetVec4(const std::string& varName, const glm::vec4& var);
	void SetInt(const std::string& varName, const int& var);
	void SetFloat(const std::string& varName, const float& var);
	void SetFloat4(const std::string& varName, aiColor4D color);
	void Link();
	void Use();
	void CleanUp();

private:
	std::string GetShaderFromFile(const char* filename);
	void AddUniformVariable(const std::string& varName);

	std::map<std::string, unsigned int> _uniformTable;
	unsigned int _programId;

};


#endif