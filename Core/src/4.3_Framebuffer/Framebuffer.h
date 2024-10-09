#pragma once

#include "Macros.h"

namespace Firefly
{
	class Framebuffer
	{
		MOVE_ONLY(Framebuffer);
	public:
		static void UnBind();
	public:
		Framebuffer(int width, int height);
		~Framebuffer();
	public:
		void PrepareForRender();
		void DirectRender(int x0 = 0, int y0 = 0, int x1 = 0, int y1 = 0);
		void Bind();
		void BindTexture(unsigned int slot = 0);
		int GetWidth() const{ return m_Width; }
		int GetHeight() const{ return m_Height; }
		const int GetRendererId() const { return m_RendererId; }
	private:
		unsigned int m_RendererId, m_Texture, m_Renderbuffer;
		int m_Width, m_Height;
	};
}