#pragma once

#include "Macros.h"
namespace Firefly
{
	class BaseFramebuffer
	{
		MOVE_ONLY(BaseFramebuffer);
	public:
		static void UnBind();
	public:
		void Bind();
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		const int GetRendererId() const { return m_RendererId; }
	protected:
		unsigned int m_RendererId;
		int m_Width, m_Height;
	protected:
		BaseFramebuffer(int width, int height);
		virtual ~BaseFramebuffer();
	};

	class Framebuffer : public BaseFramebuffer
	{
		MOVE_ONLY(Framebuffer);
	public:
		Framebuffer(int width, int height);
		virtual ~Framebuffer();
	public:
		void DirectRender(int x0 = 0, int y0 = 0, int x1 = 0, int y1 = 0);
		void BindTexture(unsigned int slot = 0);
	private:
		unsigned int m_Texture, m_Renderbuffer;
	};
}