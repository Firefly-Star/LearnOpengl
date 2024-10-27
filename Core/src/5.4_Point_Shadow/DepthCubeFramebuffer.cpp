#include "DepthCubeFramebuffer.h"

#include <utility>
#include <stdexcept>
#include <iostream>
#include <glad/glad.h>
#include "0.0_Extentions/Test/GetError.h"
namespace Firefly
{
	DepthCubeFramebuffer::DepthCubeFramebuffer(unsigned int size)
		:BaseFramebuffer(size, size), m_DepthCubeMap(0)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);
		glGenTextures(1, &m_DepthCubeMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_DepthCubeMap);
		for (int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthCubeMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDeleteTextures(1, &m_DepthCubeMap);
			glDeleteFramebuffers(1, &m_RendererId);
			throw std::runtime_error("Failed to create framebuffer!\n");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	DepthCubeFramebuffer::~DepthCubeFramebuffer()
	{
		glDeleteTextures(1, &m_DepthCubeMap);
	}

	DepthCubeFramebuffer::DepthCubeFramebuffer(DepthCubeFramebuffer&& other) noexcept
		:m_DepthCubeMap(other.m_DepthCubeMap), BaseFramebuffer(std::move(other))
	{
		other.m_DepthCubeMap = 0;
	}

	DepthCubeFramebuffer& DepthCubeFramebuffer::operator=(DepthCubeFramebuffer&& other) noexcept
	{
		if (this != &other)
		{
			BaseFramebuffer::operator=(std::move(other));
			glDeleteTextures(1, &m_DepthCubeMap);
			m_DepthCubeMap = other.m_DepthCubeMap;
			other.m_DepthCubeMap = 0;
		}
		return *this;
	}

	void DepthCubeFramebuffer::BindTexture(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_DepthCubeMap);
	}
}