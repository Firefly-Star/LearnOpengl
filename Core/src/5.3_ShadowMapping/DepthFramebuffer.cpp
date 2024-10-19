#include "DepthFramebuffer.h"

#include <glad/glad.h>
#include <iostream>

namespace Firefly
{
	DepthFramebuffer::DepthFramebuffer(DepthFramebuffer&& other) noexcept
		:BaseFramebuffer(std::move(other)), m_DepthMap(other.m_DepthMap)
	{
		other.m_DepthMap = 0;
	}

	DepthFramebuffer& DepthFramebuffer::operator=(DepthFramebuffer&& other) noexcept
	{
		if (this != &other)
		{
			BaseFramebuffer::operator=(std::move(other));

			glDeleteTextures(1, &m_DepthMap);
			m_DepthMap = other.m_DepthMap;
			other.m_DepthMap = 0;
		}

		return *this;
	}

	DepthFramebuffer::DepthFramebuffer(int width, int height)
		:BaseFramebuffer(width, height)
	{
		glGenTextures(1, &m_DepthMap);
		glBindTexture(GL_TEXTURE_2D, m_DepthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 0.0f, 0.0f, 0.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMap, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDeleteTextures(1, &m_DepthMap);
			glDeleteFramebuffers(1, &m_RendererId);
			throw std::runtime_error("Failed to create framebuffer!\n");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	DepthFramebuffer::~DepthFramebuffer()
	{
		glDeleteTextures(1, &m_DepthMap);
	}

	void DepthFramebuffer::BindTexture(int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_DepthMap);
	}
	
}