#pragma once

#include "Macros.h"
#include "4.3_Framebuffer/Framebuffer.h"

namespace Firefly
{
	class MultiSampleFramebuffer : public BaseFramebuffer
	{
		MOVE_ONLY(MultiSampleFramebuffer);
	public:
		MultiSampleFramebuffer(int width, int height, int samples);
		virtual ~MultiSampleFramebuffer();
	public:
		void CopyTo(int destRendererId);
	private:
		unsigned int m_Texture, m_Renderbuffer;
		int m_Samples;
	};
}