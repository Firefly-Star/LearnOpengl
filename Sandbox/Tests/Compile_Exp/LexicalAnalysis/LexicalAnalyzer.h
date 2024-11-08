#pragma once

#include "NFA.h"
#include "DFA.h"
#include "DFAMin.h"

namespace Firefly
{
	class LexicalAnalyzer
	{
	public:
		LexicalAnalyzer(std::string const& regex);
		~LexicalAnalyzer() = default;

		bool Analyze(std::string const&);

		StateGraph GetDFA() const { return m_DFA; }
	private:
		StateGraph m_DFA;
		DFAMin m_Minimizer;
	};
}