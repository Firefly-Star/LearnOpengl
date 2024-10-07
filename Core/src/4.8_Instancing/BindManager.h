#pragma once

#include "Macros.h"
#include <memory>
#include <mutex>

#include "1.4_Hello_Triangle/IndexBuffer.h"
#include "1.4_Hello_Triangle/Shader.h"
#include "1.4_Hello_Triangle/VertexArray.h"
#include "1.4_Hello_Triangle/VertexBuffer.h"

namespace Firefly
{
	class BindManager
	{
		SINGLETON(BindManager)
	public:
		template<typename T>
		bool CheckBind(T* object);
		template<typename T>
		void UnBind();
		template<typename T>
		T& GetCurrent();
	private:
		struct Current
		{
			IndexBuffer* ibo;
			Shader* shader;
			VertexArray* vao;
			VertexBuffer* vbo;
			Current() 
				: ibo(nullptr), shader(nullptr), vao(nullptr), vbo(nullptr) {}
		};

		Current m_Current;
	private:
		BindManager();
		~BindManager();
	};
	
}
#include "BindManager.tpp"