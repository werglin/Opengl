#include"Model.hpp"
#include "Material.hpp"

#include<iostream>

#include<glm/gtc/matrix_transform.hpp>
const std::string Model::cs_directory = "assets/fbx/";

Model::Model(glm::vec3 pos, glm::vec3 size, bool noTex):_pos(pos), _size(size), _noTex(noTex)
{

}
void Model::Init()
{

}
void Model::LoadModel(std::string name)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(cs_directory + name, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Could not load model: " << name << std::endl << importer.GetErrorString() << std::endl;
		return;
	}
	ProcessNode(scene->mRootNode, scene);
}
void Model::Render(ShaderProgram* shader)
{
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, _pos);
	transform = glm::scale(transform, _size);
	//transform = glm::rotate(transform, float(glfwGetTime()) * glm::radians(-45.0f), glm::vec3(0.5f));
	shader->SetMat4("uTransform", transform);

	for(auto &var : _meshes)
	{
		var.Render(shader);
	}
}
void Model::CleanUp()
{
	for (auto& var : _meshes)
	{
		var.CleanUp();
	}
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	Material mat;
	// process all meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		_meshes.push_back(ProcessMesh(mesh, scene));
		
		_meshes[_meshes.size() - 1]._material = Material::white_rubber;
	}

	// process child nodes 
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{

		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// process vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// pos
		vertex.pos = glm::vec3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		);

		// normals

		vertex.normal = glm::vec3(
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z
		);

		//texture uvs

		if (mesh->mTextureCoords[0]) {
			vertex.texCoord = glm::vec2(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			);
		}
		else
		{
			vertex.texCoord = glm::vec2(0.0f);
		}
		vertices.push_back(vertex);
	}
	
	// process indices
	
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// process material
	if (mesh->mMaterialIndex >= 0)
	{

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		if (_noTex)
		{
			aiColor4D diff(1.0f);
			aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diff);


			aiColor4D spec(1.0f);
			aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &spec);

			return  Mesh(vertices, indices, diff, spec);

		}
		// diffuse maps
		std::vector<Texture> diffmaps = LoadTextures(material, aiTextureType_DIFFUSE, scene);
		textures.insert(textures.end(), diffmaps.begin(), diffmaps.end());

		//specular maps

		std::vector<Texture> specmaps = LoadTextures(material, aiTextureType_SPECULAR, scene);
		textures.insert(textures.end(), specmaps.begin(), specmaps.end());

	}
	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadTextures(aiMaterial* mat, aiTextureType type, const aiScene* scene)
{
	std::vector<Texture>textures;

	aiString str;
	const aiTexture* texture;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		mat->GetTexture(type, i, &str);
		texture = scene->GetEmbeddedTexture(str.C_Str());
		
		// prevent duplicate loading
		bool skip = false;
		for (unsigned int j = 0; j < this->_loadedTextures.size(); j++)
		{
			if (std::strcmp(_loadedTextures[j]._path.data(), str.C_Str()) == 0)
			{
				textures.push_back(_loadedTextures[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			Texture tex(cs_directory, str.C_Str(), type);
			tex.Generate();
			tex.Load(texture);
			textures.push_back(tex);
			_loadedTextures.push_back(tex);
		}
	}

	return textures;
}
