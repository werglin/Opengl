#include "Texture.hpp"
#include <iostream>

#include<GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include<stb/stb_image.h>


#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

Texture::Texture(){}
Texture::Texture(std::string dir, std::string path, aiTextureType type):
	_dir(dir), _path(path),_type(type)
{
}


void Texture::Generate()
{
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);
}
void Texture::Load(bool flip){
	stbi_set_flip_vertically_on_load(flip);

	int width, height, nChannels;

	std::string fullP = _dir + _path;
	unsigned char* data = stbi_load(fullP.c_str(), &width, &height, &nChannels, 0);
	GLenum colorMode;
	switch (nChannels)
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
		glBindTexture(GL_TEXTURE_2D, _id);
		glTexImage2D(GL_TEXTURE_2D, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_LINEAR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // GL_NEAREST

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_REPEAT
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // GL_REPEAT S - x ekseni, T - y ekseni ama bu macrolar ne anlatýyo bilmiyom


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
		std::cout << "Failed to load texture :" << _dir<< _path << std::endl;
	}

	stbi_image_free(data);
}

void Texture::Load(const aiTexture* assimptexture)
{
	if (assimptexture->pcData != nullptr)
	{
		if (assimptexture->mHeight == 0)
		{
			stbi_set_flip_vertically_on_load(false);

			int width, height, nChannels;

			unsigned char* data = stbi_load_from_memory((unsigned char*)&(assimptexture->pcData[0]), assimptexture->mWidth, &width, &height, &nChannels, 0);

			GLenum colorMode;
			switch (nChannels)
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
				glBindTexture(GL_TEXTURE_2D, _id);
				// this works GL_RGB cus nChannels is 3 - search it for nChannels
				glTexImage2D(GL_TEXTURE_2D, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_LINEAR
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // GL_NEAREST

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_REPEAT
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // GL_REPEAT S - x ekseni, T - y ekseni ama bu macrolar ne anlatýyo bilmiyom


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
				std::cout << "Failed to load texture :" << _dir << _path << std::endl;
			}

			stbi_image_free(data);
		}
	}
	else
	{
		std::cout << "Failed to load texture :" << _dir << _path << std::endl;
		return;
	}

	
}
	 
void Texture::Activate(GLenum level){
	glActiveTexture(level);
	glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture::Cleanup()
{
	glDeleteTextures(1, &_id);
}

