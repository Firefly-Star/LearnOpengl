#pragma once
#include <vector>
#include <string>
#include "Macros.h"

#include "VertexBuffer.h"

namespace Firefly
{
	class VertexArray
	{
		MOVE_ONLY(VertexArray);
	public:
		enum class Type
		{
			None = 0,
			Padding,
			Int, Float, UnsignedInt, Double, Bool
		};
		struct Element
		{
			Type type;
			int count;
		};
		struct Layout
		{
			std::vector<Element> layout;
			VertexBuffer* vbo;
			int divisor = 0;
			int stride = 0;
			Layout(std::vector<Element> const& layout, VertexBuffer& vbo, int divisor = 0, int stride = 0);
			Layout(Layout const& other);
			Layout& operator=(Layout const& other);
		};
	public:
		VertexArray();
		~VertexArray();
		void Bind();

		void SetLayout(std::vector<Layout> const& layouts);

		static void UnBind();
	private:
		unsigned int m_RendererId;
		unsigned int Sizeof(Type type);
		unsigned int TypeToEnum(Type type);
		unsigned int TypeNormalized(Type type);
	};
}
