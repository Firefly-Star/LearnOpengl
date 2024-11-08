#pragma once

#include "Grammar.h"
namespace Firefly
{
	class GrammarNormalizer
	{
	public:
		using left_type = Grammar::left_type;
		using right_type = Grammar::right_type;
	public:
		explicit GrammarNormalizer(Grammar& gramma) : m_Gramma(gramma) {}
		~GrammarNormalizer() = default;
		void Normalize();
	private:
		void EliminateLeftRecursion();
		bool EliminateDirectLeftRecursion(left_type const& symbol);
		void ExtractLeftFactoring();
		left_type GenerateNewSymbol(left_type const& origin);
	private:
		Grammar& m_Gramma;
	};
}