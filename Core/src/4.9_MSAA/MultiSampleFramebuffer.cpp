#include "MultiSampleFramebuffer.h"

#include <glad/glad.h>
#include <iostream>

namespace Firefly
{
	MultiSampleFramebuffer::MultiSampleFramebuffer(MultiSampleFramebuffer&& other) noexcept
		:m_Samples(other.m_Samples), m_Texture(other.m_Texture), m_Renderbuffer(other.m_Renderbuffer), BaseFramebuffer(std::move(other))
	{
		other.m_Renderbuffer = 0;
		other.m_Texture = 0;
	}

	MultiSampleFramebuffer& MultiSampleFramebuffer::operator=(MultiSampleFramebuffer&& other) noexcept
	{
		if (this != &other)
		{
			BaseFramebuffer::operator=(std::move(other));

			glDeleteTextures(1, &m_Texture);
			glDeleteRenderbuffers(1, &m_Renderbuffer);
			
			m_Texture = other.m_Texture;
			m_Renderbuffer = other.m_Renderbuffer;
			m_Samples = other.m_Samples;
			
			other.m_Texture = 0;
			other.m_Renderbuffer = 0;
		}
		return *this;
	}

	MultiSampleFramebuffer::MultiSampleFramebuffer(int width, int height, int samples)
		: BaseFramebuffer(width, height), m_Samples(samples)
	{
		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_Texture);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA, width, height, GL_TRUE);
		glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_Texture, 0);

		glGenRenderbuffers(1, &m_Renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_Renderbuffer);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
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
	
	MultiSampleFramebuffer::~MultiSampleFramebuffer()
	{
		glDeleteTextures(1, &m_Texture);
		glDeleteRenderbuffers(1, &m_Renderbuffer);
	}
	
	void MultiSampleFramebuffer::CopyTo(int destRendererId)
	{
		glBlitNamedFramebuffer(m_RendererId, destRendererId,
			0, 0, m_Width, m_Height,
			0, 0, m_Width, m_Height,
			GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
	
}