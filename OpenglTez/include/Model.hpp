#pragma once

#ifndef MODEL_hpp
#define MODEL_hpp

#include"Mesh.hpp"

class Model {
public:
	std::vector<Mesh> _meshes;
	Model();
	void Init();
	void Render(ShaderProgram* shader);
	void CleanUp();

};

#endif // !MODEL_hpp,
