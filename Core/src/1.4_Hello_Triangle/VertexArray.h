#pragma once
#include <vector>
#include <string>

namespace Firefly
{
	class VertexArray
	{
	public:
		enum class Type
		{
			None = 0,
			Int, Float, UnsignedInt, Double, Bool
		};
		struct Element
		{
			Type type;
			int count;
		};
	public:
		VertexArray();
		VertexArray(VertexArray&& other) noexcept;
		VertexArray& operator=(VertexArray&& other) noexcept;
		~VertexArray();
		void Bind();

		void SetLayout(const std::vector<Element>& layout);

		static void UnBind();
	private:
		unsigned int m_RendererId;
		unsigned int Sizeof(Type type);
		unsigned int TypeToEnum(Type type);
		unsigned int TypeNormalized(Type type);
	};
}
