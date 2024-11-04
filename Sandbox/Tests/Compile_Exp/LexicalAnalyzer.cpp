#include "LexicalAnalyzer.h"

namespace Firefly
{
	LexicalAnalyzer::LexicalAnalyzer(std::string const& regex)
		:m_DFA(nullptr)
	{
		NFABuilder nfaBuilder;
		DFABuilder dfaBuilder;
		StateGraph nfa = nfaBuilder.Build(regex);
		StateGraph dfa = dfaBuilder.Transform(nfa);
		m_DFA = m_Minimizer.Minimize(dfa);
	}

	bool LexicalAnalyzer::Analyze(std::string const& word)
	{
		int length = word.length();
		StateNode* workNode = m_DFA.GetRoot();

		for (int i = 0; i < length; ++i)
		{
			char c = word[i];
			if (workNode->stateMap.count(c) == 0)
			{
				return false;
			}
			else
			{
				workNode = *(workNode->stateMap[c].begin());
			}
		}
		if (workNode->isAccepted)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

}