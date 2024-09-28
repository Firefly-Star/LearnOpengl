#pragma once

#include "Macros.h"

namespace Firefly
{
	class VertexBuffer
	{
		MOVE_ONLY(VertexBuffer);
	public :
		VertexBuffer(void* data, unsigned int size);
		~VertexBuffer();
		void Bind();
	private :
		unsigned int m_RendererId;
	};
}