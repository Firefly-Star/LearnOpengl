#pragma once

namespace Firefly
{
	class Framebuffer
	{
	public:
		static void UnBind();
	public:
		Framebuffer(int width, int height);
		~Framebuffer();
		Framebuffer(Framebuffer&& other) noexcept;
		Framebuffer& operator=(Framebuffer&& other) noexcept;
		Framebuffer(const Framebuffer& other) = delete;
		Framebuffer& operator=(const Framebuffer& other) = delete;
	public:
		void PrepareForRender();
		void Bind();
		void BindTexture(unsigned int slot = 0);
	private:
		unsigned int m_RendererId, m_Texture, m_Renderbuffer;
		int m_Width, m_Height;
	};
}