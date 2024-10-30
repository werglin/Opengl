#pragma once

#ifndef TEXTURE_hpp
#define TEXTURE_hpp


#include<glad/glad.h>
#include<string>


class Texture {
public:
	Texture();
	Texture(const std::string& fileName, const std::string& shaderName, bool defaultParams = true);

	void Generate();
	void Load(bool flip = true);
	
	void SetFilters(GLenum all);
	void SetFilters(GLenum mag, GLenum min);

	void SetWrap(GLenum all);
	void SetWrap(GLenum s, GLenum t);
	
	void Activate(GLenum level);

	// texture object
	int _id;
	unsigned int _tex;
	const std::string _name;
	const std::string _path;
private:
	static int s_currentId;
	static const std::string s_filepath;
	int _width;
	int _height;
	int _nChannels;
};

#endif // !TEXTURE_hpp
