#include "Texture.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <iostream>

namespace Firefly
{
	void Texture::Active(int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
	}

	Texture2D::Texture2D(const std::string& fileName, Type type)
		:Texture(fileName, type)
	{
		LoadTexture(fileName);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_RendererId);
	}

	void Texture2D::Bind(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererId);
	}

	void Texture2D::LoadTexture(const std::string& fileName)
	{
		stbi_set_flip_vertically_on_load(true);
		int height, width, channels;
		unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &channels, 0);

		if (data == nullptr)
		{
			std::cout << "Texture failed to load at path: " << fileName << "\n";
			stbi_image_free(data);
			return;
		}
		GLenum format;
		switch (channels)
		{
		case 1: format = GL_RED; break;
		case 3: format = GL_RGB; break;
		case 4: format = GL_RGBA; break;
		}

		glGenTextures(1, &m_RendererId);
		glBindTexture(GL_TEXTURE_2D, m_RendererId);

		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}