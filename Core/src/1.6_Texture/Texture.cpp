#include "Texture.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <iostream>
#include "4.8_Instancing/BindManager.h"

namespace Firefly
{
	// --------- Base Texture ----------
	void Texture::Active(int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
	}

	Texture::Texture(Texture&& other) noexcept
		:m_Path(std::move(other.m_Path)), m_RendererId(other.m_RendererId), m_Type(other.m_Type)
	{
		other.m_RendererId = 0;
	}

	Texture& Texture::operator=(Texture&& other) noexcept
	{
		if (this != &other)
		{
			glDeleteTextures(1, &m_RendererId);
			m_Path = other.m_Path;
			m_Type = other.m_Type;
			m_RendererId = other.m_RendererId;
			other.m_RendererId = 0;
		}
		return *this;
	}

	// ------------- Texture 2D -------------

	Texture2D::Texture2D(const std::string& fileName, Type type)
		:Texture(fileName, type)
	{
		LoadTexture(fileName);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_RendererId);
	}

	Texture2D::Texture2D(Texture2D&& other) noexcept
		:Texture(std::move(other))
	{
	}

	Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
	{
		if (this != &other)
		{
			Texture::operator=(std::move(other));
		}
		return *this;
	}

	void Texture2D::Bind(unsigned int slot)
	{
		if (!BindManager::GetInstance().CheckBind(this, slot))
		{
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, m_RendererId);
		}
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

	// ------------- Texture Cube ------------

	TextureCube::TextureCube(const std::vector<std::string>& fileNames)
		: Texture("", Type::Cube)
	{
		LoadTexture(fileNames);
	}

	TextureCube::~TextureCube()
	{
		glDeleteTextures(1, &m_RendererId);
	}

	TextureCube::TextureCube(TextureCube&& other) noexcept
		:Texture(std::move(other))
	{
	}

	TextureCube& TextureCube::operator=(TextureCube&& other) noexcept
	{
		if (this != &other)
		{
			Texture::operator=(std::move(other));
		}
		return *this;
	}

	void TextureCube::Bind(unsigned int slot)
	{
		if (!BindManager::GetInstance().CheckBind(this, slot))
		{
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId);
		}
	}
	
	void TextureCube::LoadTexture(const std::vector<std::string>& fileNames)
	{
		glGenTextures(1, &m_RendererId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId);

		stbi_set_flip_vertically_on_load(false);
		int width, height, channels;
		for (unsigned int i = 0; i < 6; ++i)
		{
			unsigned char* data = stbi_load(fileNames[i].c_str(), &width, &height, &channels, 0);
			if (data == nullptr)
			{
				std::cout << "Texture failed to load at path: " << fileNames[i] << "\n";
				stbi_image_free(data);
				glDeleteTextures(1, &m_RendererId);
				throw std::runtime_error("Texture failed to load at path: " + fileNames[i]);
			}
			GLenum format;
			switch (channels)
			{
			case 1: format = GL_RED; break;
			case 3: format = GL_RGB; break;
			case 4: format = GL_RGBA; break;
			}
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}