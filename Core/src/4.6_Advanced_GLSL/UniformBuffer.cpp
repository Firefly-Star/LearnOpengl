#include "UniformBuffer.h"

#include <glad/glad.h>

namespace Firefly
{
	UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept
		:m_RendererId(other.m_RendererId)
	{
		other.m_RendererId = 0;
	}

	UniformBuffer& UniformBuffer::operator=(UniformBuffer&& other) noexcept
	{
		if (this != &other)
		{
			glDeleteBuffers(1, &m_RendererId);
			m_RendererId = other.m_RendererId;
			other.m_RendererId = 0;
		}
		return *this;
	}
	
	UniformBuffer::UniformBuffer(unsigned int size)
	{
		glGenBuffers(1, &m_RendererId);
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererId);
		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	UniformBuffer::~UniformBuffer()
	{
		glDeleteBuffers(1, &m_RendererId);
	}

	void UniformBuffer::Bind(unsigned int slot)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, slot, m_RendererId);
	}

	void UniformBuffer::FillSub(unsigned int offset, size_t size, void* data)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererId);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	}

}