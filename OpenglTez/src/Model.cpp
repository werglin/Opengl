#include"Model.hpp"
#include "Material.hpp"

#include<iostream>

#include<glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtx/common.hpp>
const std::string Model::cs_directory = "assets/fbx/";

Model::Model(glm::vec3 pos, glm::vec3 size):_pos(pos), _size(size)
{

}
void Model::Init()
{

}
void Model::LoadModel(std::string name)
{
	Importer = new Assimp::Importer();
	// this link explains the problem with assimp .fbx importer 
	// https://github.com/assimp/assimp/issues/4620

	// set this to properly import mixamo rig
	Importer->SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);


	this->pScene= Importer->ReadFile(cs_directory + name, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode)
	{
		std::cout << "Could not load model: " << name << std::endl << Importer->GetErrorString() << std::endl;
		return;
	}
	_hasAnims = pScene->HasAnimations();

	this->m_GlobalInverseTransform = pScene->mRootNode->mTransformation.Inverse();


	ProcessNode(pScene->mRootNode, pScene, name);
}
void Model::IncreaseBoneId()
{
	if (m_BoneNameToIndexMap.size() == 0) // no bone
	{
		activebone = -1;
	}
	activebone++;
	if (activebone > m_BoneNameToIndexMap.size())
	{
		activebone = 0;
	}
}
void Model::Render(ShaderProgram* shader)
{
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, _pos);
	transform = glm::scale(transform, _size);
	//transform = glm::rotate(transform, float(glfwGetTime()) * glm::radians(-45.0f), glm::vec3(0.5f));
	shader->SetMat4("uTransform", transform);
	shader->SetInt("uDisplayBoneIndex", activebone);

	for(auto &var : _meshes)
	{
		var.Render(shader);
	}
}
void Model::GetBoneTransforms(float TimeInSeconds, std::vector<glm::mat4>& Transforms)
{
	float AnimationTimeTicks = TimeInSeconds;
	if (_hasAnims)
	{
		float TicksPerSecond = (float)(pScene->mAnimations[0]->mTicksPerSecond != 0 ? pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
		float TimeInTicks = TimeInSeconds * TicksPerSecond;
	    AnimationTimeTicks = fmod(TimeInTicks, (float)pScene->mAnimations[0]->mDuration);
	}

	Transforms.resize(m_BoneInfo.size());

	aiMatrix4x4 identity = aiMatrix4x4();


	ReadNodeHierarchy(AnimationTimeTicks,pScene->mRootNode, identity, 0);

	for (unsigned int i = 0; i < m_BoneInfo.size(); i++) {

		Transforms[i] = ConvertMatrixToGLMFormat(m_BoneInfo[i].FinalTransformation);
	}
}
void Model::CleanUp()
{
	for (auto& var : _meshes)
	{
		var.CleanUp();
	}

	delete this->Importer;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene, const std::string& filename)
{
	Material mat;
	// process all meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		_meshes.push_back(ProcessMesh(mesh, scene));
		
		_meshes[_meshes.size() - 1]._material = Material::white_rubber;
		_meshes[_meshes.size() - 1].name = filename + std::to_string(_meshes.size()-1);
	}

	// process child nodes 
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{

		ProcessNode(node->mChildren[i], scene, filename);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	std::vector<VertexBoneData> bonedata;
	aiColor4D diff(1.0f);
	aiColor4D spec(1.0f);

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


	bool noEmbededTex = true;
	// process material
	if (mesh->mMaterialIndex >= 0)
	{

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		
		noEmbededTex = material->GetTextureCount(aiTextureType_DIFFUSE) == 0;

		if (noEmbededTex)
		{
			aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diff);

			aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &spec);


		}
		else {

			// diffuse maps
			std::vector<Texture> diffmaps = LoadTextures(material, aiTextureType_DIFFUSE, scene);
			textures.insert(textures.end(), diffmaps.begin(), diffmaps.end());

			//specular maps

			std::vector<Texture> specmaps = LoadTextures(material, aiTextureType_SPECULAR, scene);
			textures.insert(textures.end(), specmaps.begin(), specmaps.end());
		}

	}
	bool hasBones = mesh->HasBones();

	 
	if (hasBones)
	{
		activebone = 0;
		int BoneId;
		aiBone* bone;
		bonedata.resize(vertices.size());
		std::cout << bonedata.size()<<std::endl;
		for (unsigned int i = 0; i < mesh->mNumBones; i++)
		{
			bone = mesh->mBones[i];
 			BoneId = GetBoneId(bone);

			if (BoneId == m_BoneInfo.size()) {
				BoneInfo bi(bone->mOffsetMatrix);
				m_BoneInfo.push_back(bi);
			}

			// if (bone->mNumWeights > 0)
			// {
			// 	std::cout << "Bone id " << i  << " -- Bone name "<< bone->mName.C_Str() << "  --  Bone numWeights  " << bone->mNumWeights << std::endl;
			// }
			for (unsigned int j = 0; j < bone->mNumWeights; j++)
			{
				const aiVertexWeight& vw = bone->mWeights[j];
				// here might be problem
				bonedata[vw.mVertexId].AddBoneData(BoneId, vw.mWeight);
			}
		}
	}
	else
	{
		activebone = -1;
	}

	if (noEmbededTex)
	{
		return  Mesh(vertices, indices, diff, spec, bonedata, hasBones);
	}

	return Mesh(vertices, indices, textures, bonedata, hasBones);
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

void Model::ReadNodeHierarchy(float AnimationTime,const aiNode* pNode, const aiMatrix4x4& ParentTransform, int level)
{
	std::string NodeName(pNode->mName.data);

	aiMatrix4x4 nodeTrans = pNode->mTransformation;

	// check for node hierarchy
	// for (int i = 0; i < level; i++)
	// {
	// 	std::cout << "   ";
	// }
	// std::cout << NodeName << std::endl;

	if (_hasAnims)
	{
		const aiAnimation* pAnimation = pScene->mAnimations[0];
		const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

		if (pNodeAnim) {
			// Interpolate scaling and generate scaling transformation matrix
			aiVector3D Scaling;
			CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
			aiMatrix4x4 ScalingM = aiMatrix4x4();
			ScalingM[0][0] = Scaling.x;
			ScalingM[1][1] = Scaling.y;
			ScalingM[2][2] = Scaling.z;

			// Interpolate rotation and generate rotation transformation matrix
			aiQuaternion RotationQ = aiQuaternion();
			CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
			aiMatrix4x4 RotationM = aiMatrix4x4(RotationQ.GetMatrix());

			// Interpolate translation and generate translation transformation matrix
			aiVector3D Translation;
			CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
			aiMatrix4x4 TranslationM = aiMatrix4x4();

			TranslationM[0][3] = Translation.x;
			TranslationM[1][3] = Translation.y;
			TranslationM[2][3] = Translation.z;
			TranslationM[3][3] = 1.0f;

			// Combine the above transformations
			nodeTrans = TranslationM * RotationM * ScalingM;

		}
	}

	//printf("%s - ", NodeName.c_str());

	aiMatrix4x4 GlobalTransformation = ParentTransform * nodeTrans;

	if (m_BoneNameToIndexMap.find(NodeName) != m_BoneNameToIndexMap.end()) {
		unsigned int BoneIndex = m_BoneNameToIndexMap[NodeName];
		m_BoneInfo[BoneIndex].FinalTransformation = this->m_GlobalInverseTransform *GlobalTransformation    * m_BoneInfo[BoneIndex].OffsetMatrix;
		
	}

	for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
		ReadNodeHierarchy(AnimationTime,pNode->mChildren[i], GlobalTransformation, level+1);
	}
}

int Model::GetBoneId(const aiBone* pBone)
{
	int BoneIndex = 0;
	std::string BoneName(pBone->mName.C_Str());

	if (m_BoneNameToIndexMap.find(BoneName) == m_BoneNameToIndexMap.end()) {
		// Allocate an index for a new bone
		BoneIndex = (int)m_BoneNameToIndexMap.size();
		m_BoneNameToIndexMap[BoneName] = BoneIndex;
	}
	else {
		BoneIndex = m_BoneNameToIndexMap[BoneName];
	}

	return BoneIndex;
}

void Model::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumScalingKeys == 1) {
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	unsigned int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	unsigned int NextScalingIndex = ScalingIndex + 1;
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float t1 = (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime;
	float t2 = (float)pNodeAnim->mScalingKeys[NextScalingIndex].mTime;
	float DeltaTime = t2 - t1;
	float Factor = (AnimationTime - (float)t1) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

void Model::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	unsigned int NextRotationIndex = RotationIndex + 1;
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float t1 = (float)pNodeAnim->mRotationKeys[RotationIndex].mTime;
	float t2 = (float)pNodeAnim->mRotationKeys[NextRotationIndex].mTime;
	float DeltaTime = t2 - t1;
	float Factor = (AnimationTime - t1) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out.Normalize();
}

void Model::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	unsigned int NextPositionIndex = PositionIndex + 1;
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float t1 = (float)pNodeAnim->mPositionKeys[PositionIndex].mTime;
	float t2 = (float)pNodeAnim->mPositionKeys[NextPositionIndex].mTime;
	float DeltaTime = t2 - t1;
	float Factor = (AnimationTime - t1) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

unsigned int Model::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		float t = (float)pNodeAnim->mScalingKeys[i + 1].mTime;
		if (AnimationTime < t) {
			return i;
		}
	}

	return 0;
}

unsigned int Model::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		float t = (float)pNodeAnim->mRotationKeys[i + 1].mTime;
		if (AnimationTime < t) {
			return i;
		}
	}

	return 0;
}

unsigned int Model::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		float t = (float)pNodeAnim->mPositionKeys[i + 1].mTime;
		if (AnimationTime < t) {
			return i;
		}
	}

	return 0;
}

const aiNodeAnim* Model::FindNodeAnim(const aiAnimation* pAnimation, const std::string& NodeName)
{
	for (unsigned int i = 0; i < pAnimation->mNumChannels; i++) {
		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

		if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
			return pNodeAnim;
		}
	}

	return NULL;
}
