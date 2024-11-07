#pragma once
#ifndef LIGHT_hpp
#define LIGHT_hpp

#include<glm/glm.hpp>
#include<string>

class ShaderProgram;

struct PointLight {
	glm::vec3 _position;

	float _k0;
	float _k1;
	float _k2;

	glm::vec4 _ambient;
	glm::vec4 _diffuse;
	glm::vec4 _specular;

	void Render(ShaderProgram* sp, int idx);


	const std::string _name = "uPointlight";
};

struct DirectionLight {
	glm::vec3 _direction;

	glm::vec4 _ambient;
	glm::vec4 _diffuse;
	glm::vec4 _specular;

	void Render(ShaderProgram* sp);

	const std::string _name = "uDirectionlight";
};

struct SpotLight {
	glm::vec3 _position;
	glm::vec3 _direction;

	float _k0;
	float _k1;
	float _k2;

	float _cutoff;
	float _outerCutoff;

	glm::vec4 _ambient;
	glm::vec4 _diffuse;
	glm::vec4 _specular;

	void Render(ShaderProgram* sp, int idx);

	const std::string _name = "uSpotlight";
};

#endif // !LIGHT_hpp
