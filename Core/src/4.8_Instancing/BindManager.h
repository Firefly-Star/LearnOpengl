#pragma once

#include "Macros.h"
#include <memory>
#include <mutex>
#include <vector>

#include "1.4_Hello_Triangle/IndexBuffer.h"
#include "1.4_Hello_Triangle/Shader.h"
#include "1.4_Hello_Triangle/VertexArray.h"
#include "1.4_Hello_Triangle/VertexBuffer.h"
#include "1.6_Texture/Texture.h"

namespace Firefly
{
#define MAX_TEXTURE_SLOT 32
	class BindManager
	{
		SINGLETON(BindManager)
	public:
		template<typename T>
		bool CheckBind(T* object);
		template<typename T>
		bool CheckBind(T* object, int slot);

		template<typename T>
		void UnBind();
		template<typename T>
		void UnBind(int slot);
		
		template<typename T>
		T* GetCurrent();
		template<typename T>
		T* GetCurrent(int slot);
	private:
		struct Current
		{
			IndexBuffer* ibo;
			Shader* shader;
			VertexArray* vao;
			VertexBuffer* vbo;
			std::vector<Texture*> textures;
			Current() 
				: ibo(nullptr), shader(nullptr), vao(nullptr), vbo(nullptr), textures(MAX_TEXTURE_SLOT, nullptr)
			{
			}
		};

		Current m_Current;
	private:
		BindManager();
		~BindManager();
	};
#undef MAX_TEXTURE_SLOT
}
#include "BindManager.tpp"