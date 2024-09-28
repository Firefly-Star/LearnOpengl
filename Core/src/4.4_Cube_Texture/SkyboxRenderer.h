#pragma once

#include <memory>
#include <utility>
#include <mutex>

#include "Macros.h"
#include "1.4_Hello_Triangle/IndexBuffer.h"
#include "1.4_Hello_Triangle/VertexArray.h"
#include "1.4_Hello_Triangle/VertexBuffer.h"

using namespace std;
namespace Firefly
{
	class SkyboxRenderer
	{
		SINGLETON(SkyboxRenderer);
	public:
		void Render();
	private:
		VertexArray m_VAO;
		VertexBuffer m_VBO;
		IndexBuffer m_IBO;
	private:
		SkyboxRenderer();
		~SkyboxRenderer();
	};
}