#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "VertexArray.h"

#include "4.8_Instancing/BindManager.h"

namespace Firefly
{
	VertexArray::Layout::Layout(std::vector<Element> const& layout, VertexBuffer& vbo, int divisor, int stride)
		: layout(layout), vbo(&vbo), divisor(divisor), stride(stride)
	{
	}

	VertexArray::Layout::Layout(Layout const& other)
		: layout(other.layout), vbo(other.vbo), divisor(other.divisor), stride(other.stride)
	{
	}

	VertexArray::Layout& VertexArray::Layout::operator=(Layout const& other)
	{
		if (this != &other)
		{
			layout = other.layout;
			vbo = other.vbo;
			divisor = other.divisor;
			stride = other.stride;
		}
		return *this;
	}

	VertexArray::VertexArray()
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
		if (!BindManager::GetInstance().CheckBind(this))
			glBindVertexArray(m_RendererId);
	}
	
	void VertexArray::SetLayout(std::vector<Layout> const& layouts)
	{
		Bind();
		int n = static_cast<int>(layouts.size());
		int beginIndex = 0;
		for (int i = 0; i < n; ++i)
		{
			int stride = layouts[i].stride;
			int size = static_cast<int>(layouts[i].layout.size());
			if (stride == 0)
			{
				for (int j = 0; j < size; ++j)
				{
					stride += layouts[i].layout[j].count * Sizeof(layouts[i].layout[j].type);
				}
			}
			layouts[i].vbo->Bind();
			size_t offset = 0;
			for (int j = 0; j < size; ++j)
			{
				if (layouts[i].layout[j].type != Type::Padding)
				{
					glVertexAttribPointer(beginIndex, layouts[i].layout[j].count, TypeToEnum(layouts[i].layout[j].type), TypeNormalized(layouts[i].layout[j].type), stride, reinterpret_cast<void*>(offset));
					glEnableVertexAttribArray(beginIndex);
					if (layouts[i].divisor != 0)
					{
						glVertexAttribDivisor(beginIndex, layouts[i].divisor);
					}
					++beginIndex;
				}
				offset += layouts[i].layout[j].count * Sizeof(layouts[i].layout[j].type);
			}
		}
	}

	void VertexArray::UnBind()
	{
		glBindVertexArray(0);
			BindManager::GetInstance().UnBind<VertexArray>();
	}

	unsigned int VertexArray::Sizeof(Type type)
	{
		switch (type)
		{
		case Type::Padding: return 1;
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