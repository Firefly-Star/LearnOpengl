#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "VertexArray.h"

namespace Firefly
{
	Firefly::VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_RendererId);
		glBindVertexArray(m_RendererId);
	}

	VertexArray::VertexArray(VertexArray&& other) noexcept
		:m_RendererId(other.m_RendererId)
	{
		other.m_RendererId = 0;
	}

	VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
	{
		if (this != &other)
		{
			glDeleteVertexArrays(1, &m_RendererId);
			m_RendererId = other.m_RendererId;
			other.m_RendererId = 0;
		}
		return *this;
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererId);
	}
	
	void VertexArray::Bind()
	{
		glBindVertexArray(m_RendererId);
	}
	
	void VertexArray::SetLayout(const std::vector<Element>& layout)
	{
		Bind();
		int stride = 0;
		for (int i = 0; i < layout.size(); i++)
		{
			stride += layout[i].count * Sizeof(layout[i].type);
		}
		size_t offset = 0;
		for (int i = 0; i < layout.size(); i++)
		{
			glVertexAttribPointer(i, layout[i].count, TypeToEnum(layout[i].type), TypeNormalized(layout[i].type), stride, reinterpret_cast<void*>(offset));
			glEnableVertexAttribArray(i);
			offset += layout[i].count * Sizeof(layout[i].type);
		}
	}

	void VertexArray::UnBind()
	{
		glBindVertexArray(0);
	}

	unsigned int VertexArray::Sizeof(Type type)
	{
		switch (type)
		{
		case Type::Int: return sizeof(int);
		case Type::Float: return sizeof(float);
		case Type::Bool: return sizeof(bool);
		case Type::Double: return sizeof(double);
		case Type::UnsignedInt: return sizeof(unsigned int);
		}
		return 0;
	}

	unsigned int VertexArray::TypeToEnum(Type type)
	{
		switch (type)
		{
		case Type::Int: return GL_INT;
		case Type::Float: return GL_FLOAT;
		case Type::Bool: return GL_BOOL;
		case Type::Double: return GL_DOUBLE;
		case Type::UnsignedInt: return GL_UNSIGNED_INT;
		}
		return 0;
	}

	unsigned int VertexArray::TypeNormalized(Type type)
	{
		switch (type)
		{
		case Type::Int: 
		case Type::Float:
		case Type::Bool: 
		case Type::Double:
		case Type::UnsignedInt: return GL_FALSE;
		}
		return 0;
	}
	
}