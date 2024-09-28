#pragma once
#include <vector>
#include <string>
#include "Macros.h"
namespace Firefly
{
	class VertexArray
	{
		MOVE_ONLY(VertexArray);
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
