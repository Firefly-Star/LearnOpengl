#include "Framebuffer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace Firefly
{
	Framebuffer::Framebuffer(int width, int height)
		:m_Width(width), m_Height(height)
	{
		glGenFramebuffers(1, &m_RendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);

		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);

		glGenRenderbuffers(1, &m_Renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_Renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_Renderbuffer);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

			glDeleteTextures(1, &m_Texture);
			glDeleteRenderbuffers(1, &m_Renderbuffer);
			glDeleteFramebuffers(1, &m_RendererId);
			throw std::runtime_error("Failed to create framebuffer!\n");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteTextures(1, &m_Texture);
		glDeleteRenderbuffers(1, &m_Renderbuffer);
		glDeleteFramebuffers(1, &m_RendererId);
	}

	Framebuffer::Framebuffer(Framebuffer&& other) noexcept
		:m_RendererId(other.m_RendererId), m_Texture(other.m_Texture), m_Renderbuffer(other.m_Renderbuffer),
		m_Height(other.m_Height), m_Width(other.m_Width)
	{
		other.m_RendererId = 0;
		other.m_Renderbuffer = 0;
		other.m_Texture = 0;
	}

	Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept
	{
		if (this != &other)
		{
			glDeleteTextures(1, &m_Texture);
			glDeleteRenderbuffers(1, &m_Renderbuffer);
			glDeleteFramebuffers(1, &m_RendererId);

			m_RendererId = other.m_RendererId;
			m_Texture = other.m_Texture;
			m_Renderbuffer = other.m_Renderbuffer;
			m_Height = other.m_Height;
			m_Width = other.m_Width;

			other.m_RendererId = 0;
			other.m_Renderbuffer = 0;
			other.m_Texture = 0;
		}
		return *this;
	}

	void Framebuffer::PrepareForRender()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);
	}

	void Framebuffer::DirectRender(int x0, int y0, int x1, int y1)
	{
		glBlitNamedFramebuffer(m_RendererId, 0, 
			x0, y0, x1 == 0 ? m_Width : x1, y1 == 0 ? m_Height : y1,
			x0, y0, x1 == 0 ? m_Width : x1, y1 == 0 ? m_Height : y1,
			GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	void Framebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);
	}

	void Framebuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::BindTexture(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
	}
}