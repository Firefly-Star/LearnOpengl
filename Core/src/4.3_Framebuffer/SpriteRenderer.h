#pragma once

#include <memory>
#include <mutex>

#include "Macros.h"
#include "1.4_Hello_Triangle/IndexBuffer.h"
#include "1.4_Hello_Triangle/VertexArray.h"
#include "1.4_Hello_Triangle/VertexBuffer.h"

namespace Firefly
{
	class SpriteRenderer
	{
		SINGLETON(SpriteRenderer);
	public:
		void Render();
	private:
		VertexArray m_VAO;
		IndexBuffer m_IBO;
		VertexBuffer m_VBO;
	private:
		SpriteRenderer();
		~SpriteRenderer();
	};
}