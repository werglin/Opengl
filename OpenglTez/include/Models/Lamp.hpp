#pragma once
#ifndef LAMP_hpp
#define LAMP_hpp

#include "Cube.hpp"
#include "Light.hpp"


class Lamp :public Cube {
public:
	glm::vec3 _lightColor;
			  
	PointLight _pointlight;
	Lamp()
	{
	}

	Lamp(glm::vec3 lightcol,
		glm::vec4 ambient,
		glm::vec4 diffuse,
		glm::vec4 specular,
		float k0,
		float k1,
		float k2,
		glm::vec3 pos,
		glm::vec3 size) :_lightColor(lightcol), _pointlight({pos, k0,k1,k2, ambient, diffuse, specular}), Cube(Material::white_plastic, pos, size)
	{
	}
	//I know what I'm doing, compiler, use the default version.
	Lamp(const Lamp&) = default;

	void Render(ShaderProgram* shader)
	{
		shader->SetVec3("lightColor", _lightColor);
		Cube::Render(shader);
	}
};

#endif // !LAMP_hpp
