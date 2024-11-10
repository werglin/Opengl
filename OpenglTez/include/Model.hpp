#pragma once

#ifndef MODEL_hpp
#define MODEL_hpp

#include"Mesh.hpp"

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#include<glm/glm.hpp>

class Model {
public:
	glm::vec3 _pos;
	glm::vec3 _size;
	bool _hasAnims;
	
	Model(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 size = glm::vec3(0.0f));

	void Init();
	void LoadModel(std::string name);
	void IncreaseBoneId();
	void Render(ShaderProgram* shader);
	void GetBoneTransforms(float TimeInSeconds,std::vector<glm::mat4>& Transforms);
	void CleanUp();

protected:

	const static std::string cs_directory;

	std::vector<Mesh> _meshes;
	std::vector<Texture> _loadedTextures;

	void ProcessNode(aiNode* node, const aiScene* scene, const std::string& filename);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadTextures(aiMaterial* mat, aiTextureType type, const aiScene* scene);
private:
	glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
	{
		glm::mat4 to;
		//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
	}
	void ReadNodeHierarchy(float AnimationTime, const aiNode* pNode, const aiMatrix4x4& ParentTransform);
	struct BoneInfo
	{
		aiMatrix4x4 OffsetMatrix;
		aiMatrix4x4 FinalTransformation;

		BoneInfo(const aiMatrix4x4& Offset)
		{
			OffsetMatrix = Offset;
			FinalTransformation = aiMatrix4x4();
		}
	};

	Assimp::Importer* Importer = NULL;
	const aiScene* pScene = NULL;

	int activebone = 0;

	int GetBoneId(const aiBone* pBone);

	std::vector<BoneInfo> m_BoneInfo;
	std::map<std::string, unsigned int> m_BoneNameToIndexMap;




	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string& NodeName);
	aiMatrix4x4 m_GlobalInverseTransform;
};

#endif // !MODEL_hpp,
