#pragma once
#ifndef LAMP_hpp
#define LAMP_hpp

#include "Cube.hpp"

class Lamp :public Cube {
public:
	glm::vec3 _lightColor;
			  
	glm::vec3 _ambient;
	glm::vec3 _diffuse;
	glm::vec3 _specular;

	Lamp(glm::vec3 lightcol,
		glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular,
		glm::vec3 pos,
		glm::vec3 size) :_lightColor(lightcol), _ambient(ambient), _diffuse(diffuse), _specular(specular),Cube(Material::white_plastic, pos, size)
	{

	}

	void Render(ShaderProgram* shader)
	{
		shader->SetVec3("lightColor", _lightColor);
		Cube::Render(shader);
	}
};

#endif // !LAMP_hpp
