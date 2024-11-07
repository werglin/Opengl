#pragma once

#ifndef MODEL_hpp
#define MODEL_hpp

#include"Mesh.hpp"

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

class Model {
public:
	glm::vec3 _pos;
	glm::vec3 _size;
	
	Model(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 size = glm::vec3(0.0f), bool noTex = false);

	void Init();
	void LoadModel(std::string name);
	void Render(ShaderProgram* shader);
	void CleanUp();

protected:

	bool _noTex;
	const static std::string cs_directory;

	std::vector<Mesh> _meshes;
	std::vector<Texture> _loadedTextures;

	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadTextures(aiMaterial* mat, aiTextureType type, const aiScene* scene);

};

#endif // !MODEL_hpp,
