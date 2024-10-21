#pragma once

#include "Macros.h"
#include "1.4_Hello_Triangle/VertexArray.h"
#include "1.4_Hello_Triangle/VertexBuffer.h"
#include "1.4_Hello_Triangle/IndexBuffer.h"
#include "1.9_Camera/View.h"
#include <mutex>
#include <memory>

namespace Firefly
{
	class AxisRenderer
	{
		SINGLETON(AxisRenderer);
	public:
		void Render(const View& view, float x, float y, float size = 1.0f);
	private:
		VertexArray m_VAO;
		IndexBuffer m_IBO;
		VertexBuffer m_VBO;
	private:
		AxisRenderer();
		~AxisRenderer();
	};
}