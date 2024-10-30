#include "Texture.hpp"
#include <iostream>

#include<GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include<stb/stb_image.h>

int Texture::s_currentId = 0;
const std::string Texture::s_filepath = "assets/textures/";
Texture::Texture(){}
Texture::Texture(const std::string& fileName, const std::string& shaderName, bool defaultParams):
	_path(s_filepath + fileName), _name(shaderName), _id(s_currentId++)
{
	Generate();
	Load(true);

	if (defaultParams)
	{
		SetFilters(GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
		SetWrap(GL_REPEAT);
	}
}

void Texture::Generate()
{
	glGenTextures(1, &_tex);
	glBindTexture(GL_TEXTURE_2D, _tex);
}
void Texture::Load(bool flip){
	stbi_set_flip_vertically_on_load(flip);

	unsigned char* data = stbi_load(_path.c_str(), &_width, &_height, &_nChannels, 0);

	GLenum colorMode;
	switch (_nChannels)
	{
	case 1:
		colorMode = GL_RED;
		break;
	case 4:
		colorMode = GL_RGBA;
		break;
	default:
		colorMode = GL_RGB;
		break;
	}

	if (data != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, _tex);
		glTexImage2D(GL_TEXTURE_2D, 0, colorMode, _width, _height, 0, colorMode, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// if check needed
		// if (_tex == 0) {
		// 	std::cout << "Error: Texture " << _name << " failed to generate a valid texture ID.\n";
		// }
		// else {
		// 	std::cout << "Texture " << _name << " loaded with ID: " << _tex << std::endl;
		// }
	}
	else
	{
		std::cout << "Failed to load texture :" << _name << std::endl;
	}

	stbi_image_free(data);
}
	 
void Texture::SetFilters(GLenum all){
	SetFilters(all, all);
}
void Texture::SetFilters(GLenum mag, GLenum min){
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag); // GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min); // GL_NEAREST
}
	 
void Texture::SetWrap(GLenum all){
	SetWrap(all, all);
}
void Texture::SetWrap(GLenum s, GLenum t){
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s); // GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t); // GL_REPEAT S - x ekseni, T - y ekseni ama bu macrolar ne anlatýyo bilmiyom
}
	 
void Texture::Activate(GLenum level){
	glActiveTexture(level);
	glBindTexture(GL_TEXTURE_2D, _tex);
}