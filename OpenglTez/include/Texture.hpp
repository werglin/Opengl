#pragma once

#ifndef TEXTURE_hpp
#define TEXTURE_hpp


#include<glad/glad.h>
#include<assimp/scene.h>
#include<string>

class aiTexture;

class Texture {
public:
	Texture();
	Texture(std::string dir, std::string path, aiTextureType type);

	void Generate();
	void Load(bool flip = true);
	void Load(const aiTexture* assimptexture);
	
	void Activate(GLenum level);

	void Cleanup();

	unsigned int _id;
	aiTextureType _type;
	std::string _dir;
	std::string _path;
};

#endif // !TEXTURE_hpp
