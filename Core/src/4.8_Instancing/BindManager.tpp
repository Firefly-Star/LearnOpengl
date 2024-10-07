#include <iostream>
namespace Firefly
{
	template<typename T>
	bool BindManager::CheckBind(T* object)
	{
		bool result;
		if constexpr (std::is_same_v<T, IndexBuffer>)
		{
			result = object == m_Current.ibo;
			if (!result) m_Current.ibo = object;
		}
		else if constexpr (std::is_same_v<T, Shader>)
		{
			result = object == m_Current.shader;
			if (!result) m_Current.shader = object;
		}
		else if constexpr (std::is_same_v<T, VertexArray>)
		{
			result = object == m_Current.vao;
			if (!result) m_Current.vao = object;
		}
		else if constexpr (std::is_same_v<T, VertexBuffer>)
		{
			result = object == m_Current.vbo;
			if (!result) m_Current.vbo = object;
		}
		return result;
	}

	template<typename T>
	void BindManager::UnBind()
	{
		if constexpr (std::is_same_v<T, IndexBuffer>)
		{
			m_Current.ibo = nullptr;
		}
		else if constexpr (std::is_same_v<T, Shader>)
		{
			m_Current.shader = nullptr;
		}
		else if constexpr(std::is_same_v<T, VertexArray>)
		{
			m_Current.vao = nullptr;
		}
		else if constexpr (std::is_same_v<T, VertexBuffer>)
		{
			m_Current.vbo = nullptr;
		}
	}
	
	template<typename T>
	T& BindManager::GetCurrent()
	{
		if constexpr (std::is_same_v<T, IndexBuffer>)
		{
			return m_Current.ibo;
		}
		if constexpr (std::is_same_v<T, Shader>)
		{
			return m_Current.shader;
		}
		else if constexpr (std::is_same_v<T, VertexArray>)
		{
			return m_Current.vao;
		}
		if constexpr (std::is_same_v<T, VertexBuffer>)
		{
			return m_Current.vbo;
		}
	}
}