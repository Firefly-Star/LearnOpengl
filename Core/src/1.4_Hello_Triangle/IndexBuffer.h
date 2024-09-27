#pragma once

namespace Firefly
{
	class IndexBuffer
	{
	public:
		IndexBuffer(void* data, unsigned int size);
		IndexBuffer(IndexBuffer&& other) noexcept;
		IndexBuffer& operator=(IndexBuffer&& other) noexcept;
		~IndexBuffer();
		void Bind();
		unsigned int GetCount() { return m_Count; }
	private:
		unsigned int m_RendererId;
		unsigned int m_Count;
	};

}

