#pragma once

#include "Macros.h"
#include "4.3_Framebuffer/Framebuffer.h"

namespace Firefly
{
	class DepthCubeFramebuffer : public BaseFramebuffer
	{
		MOVE_ONLY(DepthCubeFramebuffer);
	public:
		DepthCubeFramebuffer(unsigned int size);
		~DepthCubeFramebuffer();

		void BindTexture(unsigned int slot = 0);
	private:
		unsigned int m_DepthCubeMap;
	};
}