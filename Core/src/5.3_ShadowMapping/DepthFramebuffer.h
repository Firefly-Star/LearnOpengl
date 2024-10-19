#pragma once

#include "Macros.h"
#include "4.3_Framebuffer/Framebuffer.h"

#include <utility>

namespace Firefly
{
	class DepthFramebuffer : public BaseFramebuffer
	{
		MOVE_ONLY(DepthFramebuffer);
	public:
		DepthFramebuffer(int width, int height);
		~DepthFramebuffer();
		void BindTexture(int slot = 0);
	private:
		unsigned int m_DepthMap;
	};
}