#include "Light.hpp"
#include "ShaderProgram.hpp"


void PointLight::Render(ShaderProgram* sp, int idx)
{
	std::string name = _name + "[" + std::to_string(idx) + "]";
	sp->SetVec3(name + ".pos", _position);

	sp->SetFloat(name + ".k0", _k0);
	sp->SetFloat(name + ".k1", _k1);
	sp->SetFloat(name + ".k2", _k2);

	sp->SetVec3(name + ".ambient", _ambient);
	sp->SetVec3(name + ".diffuse", _diffuse);
	sp->SetVec3(name + ".specular", _specular);


}

void DirectionLight::Render(ShaderProgram* sp)
{
	sp->SetVec3(_name + ".direction", _direction);

	sp->SetFloat(_name + ".k0", _k0);
	sp->SetFloat(_name + ".k1", _k1);
	sp->SetFloat(_name + ".k2", _k2);

	sp->SetVec3(_name + ".ambient", _ambient);
	sp->SetVec3(_name + ".diffuse", _diffuse);
	sp->SetVec3(_name + ".specular", _specular);


}

void SpotLight::Render(ShaderProgram* sp, int idx)
{
	std::string name = _name + "[" + std::to_string(idx) + "]";
	sp->SetVec3(name + ".pos", _position);
	sp->SetVec3(name + ".direction", _direction);

	sp->SetFloat(name + ".k0", _k0);
	sp->SetFloat(name + ".k1", _k1);
	sp->SetFloat(name + ".k2", _k2);

	sp->SetFloat(name + ".cutoff", _cutoff);
	sp->SetFloat(name + ".outer_cutoff", _outerCutoff);

	sp->SetVec3(name + ".ambient", _ambient);
	sp->SetVec3(name + ".diffuse", _diffuse);
	sp->SetVec3(name + ".specular", _specular);
}