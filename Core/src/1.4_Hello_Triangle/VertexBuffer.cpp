#include "VertexBuffer.h"
#include <glad/glad.h>
#include "4.8_Instancing/BindManager.h"

namespace Firefly
{
	VertexBuffer::VertexBuffer(void* data, unsigned int size)
	{
		glGenBuffers(1, &m_RendererId);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
		:m_RendererId(other.m_RendererId)
	{
		other.m_RendererId = 0;
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
	{
		if (this != &other)
		{
			glDeleteBuffers(1, &m_RendererId);
			m_RendererId = other.m_RendererId;
			other.m_RendererId = 0;
		}
		return *this;
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererId);
	}

	void VertexBuffer::Bind()
	{
		if(!BindManager::GetInstance().CheckBind(this))
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
	}

	void VertexBuffer::UnBind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		BindManager::GetInstance().UnBind<VertexBuffer>();
	}
}