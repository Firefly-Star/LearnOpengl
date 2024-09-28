#pragma once

#include "Macros.h"

namespace Firefly
{
	class IndexBuffer
	{
		MOVE_ONLY(IndexBuffer);
	public:
		IndexBuffer(void* data, unsigned int size);
		~IndexBuffer();
		void Bind();
		unsigned int GetCount() { return m_Count; }
	private:
		unsigned int m_RendererId;
		unsigned int m_Count;
	};

}

