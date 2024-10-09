#pragma once

#include "Macros.h"
#include "4.3_Framebuffer/Framebuffer.h"

namespace Firefly
{
	class MultiSampleFramebuffer
	{
		MOVE_ONLY(MultiSampleFramebuffer);
	public:
		static void UnBind();
	public:
		MultiSampleFramebuffer(int width, int height, int samples);
		~MultiSampleFramebuffer();
	public:
		void PrepareForRender();
		void CopyTo(int destRendererId);
		void Bind();
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
	private:
		unsigned int m_RendererId, m_Texture, m_Renderbuffer;
		int m_Width, m_Height, m_Samples;
	};
}