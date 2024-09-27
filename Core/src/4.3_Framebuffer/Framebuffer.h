#pragma once

namespace Firefly
{
	class Framebuffer
	{
	public:
		Framebuffer(int width, int height);
		void WriteIn();
		void Bind();
	private:
		unsigned int m_RendererId, m_Texture, m_Renderbuffer;
		int m_Width, m_Height;
	};
}