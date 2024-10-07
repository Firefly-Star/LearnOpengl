#include "IndexBuffer.h"
#include "4.8_Instancing/BindManager.h"
#include <glad/glad.h>

namespace Firefly
{
	IndexBuffer::IndexBuffer(void* data, unsigned int size)
		:m_Count(size / sizeof(unsigned int))
	{
		glGenBuffers(1, &m_RendererId);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
		:m_RendererId(other.m_RendererId), m_Count(other.m_Count)
	{
		other.m_RendererId = 0;
	}
	
	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
	{
		if (this != &other)
		{
			glDeleteBuffers(1, &m_RendererId);
			m_RendererId = other.m_RendererId;
			m_Count = other.m_Count;
			other.m_RendererId = 0;
		}
		return *this;
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererId);
	}
	
	void IndexBuffer::Bind()
	{
		if(!BindManager::GetInstance().CheckBind(this))
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
	}

	void IndexBuffer::UnBind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		BindManager::GetInstance().UnBind<IndexBuffer>();
	}
}