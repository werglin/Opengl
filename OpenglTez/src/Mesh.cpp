#include"Mesh.hpp"


#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

std::vector<struct Vertex> Vertex::GenList(float* vertices, int size_vertices)
{
	std::vector<Vertex> ret(size_vertices);
	int stride = sizeof(Vertex) / sizeof(float);

	for (int i = 0; i < size_vertices; i++)
	{
		ret[i].pos = glm::vec3(
			vertices[i * stride + 0],
			vertices[i * stride + 1],
			vertices[i * stride + 2]
			);
		ret[i].texCoord = glm::vec2(
			vertices[i * stride + 3],
			vertices[i * stride + 4]
		);
	}

	return ret;
}

Mesh::Mesh(){}
Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures, ShaderProgram* sp):
	_vertices(vertices), _indices(indices), _textures(textures)
{
	Setup();
	sp->Use();
	int i;
	for ( i = 0; i < _textures.size(); i++)
	{
		sp->AddUniformVariable(_textures[i]._name);
		sp->SetInt(_textures[i]._name, i);

	}
	// frees vao on memory
	glBindVertexArray(0);
}

void Mesh::Render(ShaderProgram* shader){

	shader->Use();
	for (unsigned int i = 0; i < _textures.size(); i++)
	{
		_textures[i].Activate(GL_TEXTURE0 + i);
		// if needed test
		// int unit;
		// glGetUniformiv(shader->GetProgramId(), glGetUniformLocation(shader->GetProgramId(), _textures[i]._name.c_str()), &unit);
		// std::cout << "Uniform " << _textures[i]._name << " assigned to texture unit " << unit << std::endl;
	}
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::CleanUp()
{
	glDeleteVertexArrays(1,&_vao);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
}

void Mesh::Setup(){
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);
	// setup ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);


	// set attrib pointer
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, pos)));

	// set attrib pointer
	// 3*sizeof(float) olmas� laz�m ��nk� 3 integer de�er d�nd�r�yor
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoord)));


}
