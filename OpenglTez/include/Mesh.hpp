#pragma once

#ifndef MESH_hpp
#define MESH_hpp


#include<vector>
#include<glm/glm.hpp>

#include"Material.hpp"
#include"ShaderProgram.hpp"
#include"Texture.hpp"


struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoord;

	static std::vector<struct Vertex> GenList(float* vertices, int size_vertices);
};
typedef struct Vertex Vertex;

class Mesh {
public:
	std::vector<Vertex> _vertices;
	std::vector<unsigned int> _indices;
	unsigned int _vao;

	Material _material;

	std::vector<Texture> _textures;
	aiColor4D _diffuse;
	aiColor4D _specular;
	
	Mesh();
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures = {});
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, aiColor4D diffuse, aiColor4D specular);

	void Render(ShaderProgram* shader);

	void CleanUp();

private:
	unsigned int _vbo, _ebo;

	bool noTex;

	void Setup();
};

#endif // !MESH_hpp
