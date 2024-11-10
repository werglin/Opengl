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
		ret[i].normal = glm::vec3(
			vertices[i * stride + 3],
			vertices[i * stride + 4],
			vertices[i * stride + 5]
			);
		ret[i].texCoord = glm::vec2(
			vertices[i * stride + 6],
			vertices[i * stride + 7]
		);
	}

	return ret;
}


Mesh::Mesh(){}
Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures, const std::vector<VertexBoneData>& vertexbonedata, bool hasBones) :
	_vertices(vertices), _indices(indices), _textures(textures),_boneDatas(vertexbonedata), _hasBones(hasBones)
{
	noEmbededTex = false;
	Setup();
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, aiColor4D diffuse, aiColor4D specular, const std::vector<VertexBoneData>& vertexbonedata, bool hasBones) :
	_vertices(vertices), _indices(indices),_diffuse(diffuse), _specular(specular), _boneDatas(vertexbonedata), _hasBones(hasBones)
{
	noEmbededTex = true;
	Setup();
}

void Mesh::Render(ShaderProgram* shader){

		// if needed test
		// int unit;
		// glGetUniformiv(shader->GetProgramId(), glGetUniformLocation(shader->GetProgramId(), _textures[i]._name.c_str()), &unit);
		// std::cout << "Uniform " << _textures[i]._name << " assigned to texture unit " << unit << std::endl;
	if (noEmbededTex)
	{
		shader->SetVec4("uMaterial.ambient", glm::vec4(_material.ambient, 1.0f));
		shader->SetFloat4("uMaterial.diffuse", _diffuse);
		shader->SetFloat4("uMaterial.specular", _specular);
		shader->SetFloat("uMaterial.smoothness", _material.smoothness);
		shader->SetInt("noEmbededTex", 1);
	}
	else
	{
		shader->SetVec4("uMaterial.ambient", glm::vec4(_material.ambient, 1.0f));
		shader->SetVec4("uMaterial.diffuse", glm::vec4(_material.diffuse, 1.0f));
		shader->SetVec4("uMaterial.specular", glm::vec4(_material.specular, 1.0f));
		shader->SetFloat("uMaterial.smoothness", _material.smoothness);
		unsigned int diffuseIdx = 1;
		unsigned int specIdx = 1;
		shader->SetInt("noEmbededTex", 0);

		for (unsigned int i = 0; i < _textures.size(); i++)
		{
			std::string name;
			switch (_textures[i]._type)
			{
			case aiTextureType_DIFFUSE:
				name = "diffuse_texture" + std::to_string(diffuseIdx++);
				break;
			case aiTextureType_SPECULAR:
				name = "specular_texture" + std::to_string(specIdx++);
			}
			shader->SetInt(name, i);
			//activate texture
			_textures[i].Activate(GL_TEXTURE0 + i);
		}
	}

	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);

	
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}


void Mesh::CleanUp()
{
	for (int i = 0; i < _textures.size(); i++)
	{
		_textures[i].Cleanup();
	}
	glDeleteVertexArrays(1,&_vao);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);

	if(_hasBones)
		glDeleteBuffers(1, &_bbo);
}

void Mesh::Setup(){
	// frees vao on memory
	glBindVertexArray(0);

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
	// 3*sizeof(float) olmasý lazým çünkü 3 integer deðer döndürüyor
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoord)));

	if (_hasBones)
	{
		glGenBuffers(1, &_bbo);

		glBindBuffer(GL_ARRAY_BUFFER, _bbo);
		glBufferData(GL_ARRAY_BUFFER, _boneDatas.size() * sizeof(VertexBoneData), &_boneDatas[0], GL_STATIC_DRAW);

		// bone attrib
		glEnableVertexAttribArray(3);
		glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (void*)(offsetof(VertexBoneData, boneids)));

		// weight attrib
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (void*)(offsetof(VertexBoneData, weights)));

	}
	else
	{
		glVertexAttribI4i(3, 0, 0, 0, 0);  // Default bone IDs
		glVertexAttrib4f(4, 0.0f, 0.0f, 0.0f, 0.0f); // Default weights
	}

	

	// frees vao on memory
	glBindVertexArray(0);

}

void VertexBoneData::AddBoneData(unsigned int BoneID, float Weight)
{
	for (unsigned int i = 0; i < 4; i++) {
		if (weights[i] == 0.0) {
			boneids[i] = BoneID;
			weights[i] = Weight;
			//printf("Adding bone %d weight %f at index %i\n", BoneID, Weight, i);
			return;
		}
	}
}
