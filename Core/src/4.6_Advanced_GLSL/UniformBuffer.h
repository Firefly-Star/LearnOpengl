#pragma once

#include "Macros.h"

namespace Firefly
{
	class UniformBuffer
	{
		MOVE_ONLY(UniformBuffer);
	public:
		UniformBuffer(unsigned int size);
		~UniformBuffer();
		void Bind(unsigned int slot = 0);
		void FillSub(unsigned int offset, size_t size, void* data);
	private:
		unsigned int m_RendererId;
	};
}