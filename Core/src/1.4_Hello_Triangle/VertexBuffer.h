#pragma once

namespace Firefly
{
	class VertexBuffer
	{
	public :
		VertexBuffer(void* data, unsigned int size);
		VertexBuffer(VertexBuffer&& other) noexcept;
		VertexBuffer& operator=(VertexBuffer&& other) noexcept;
		~VertexBuffer();
		void Bind();
	private :
		unsigned int m_RendererId;
	};
}