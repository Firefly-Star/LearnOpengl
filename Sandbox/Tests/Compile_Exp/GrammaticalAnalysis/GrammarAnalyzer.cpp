#include "GrammarAnalyzer.h"
#include "GrammarNormalizer.h"
#include <stack>

namespace Firefly
{
	GrammarAnalyzer::GrammarAnalyzer(Grammar const& grammar)
		:m_Grammar(grammar)
	{
		GrammarNormalizer normalizer(m_Grammar);
		normalizer.Normalize();
		m_GrammarSetCalculator.reset(new GrammarSetCalculator(m_Grammar));
	}

	bool GrammarAnalyzer::CheckIfLL1() const
	{
		std::unordered_map<left_type, std::vector<set_type>> const& selectSets = m_GrammarSetCalculator->GetSelectSet();
		for (auto const& [left, sets] : selectSets)
		{
			set_type record;
			for (set_type const& set : sets)
			{
				for (member_type const& member : set)
				{
					if (!record.insert(member).second)
					{
						return false;
					}
				}
			}
		}
		return true;
	}

	bool GrammarAnalyzer::Analyze(std::string const& sentence) const
	{
		std::stack<char> input;
		std::stack<member_type> analyzeStack;
		input.push('$');
		for (int i = sentence.length() - 1; i >= 0; --i)
		{
			input.push(sentence[i]);
		}
		analyzeStack.push('$');
		analyzeStack.push(m_Grammar.GetStartSymbol());

		std::unordered_map<left_type, std::vector<right_type>> const& productions = m_Grammar.GetProductions();
		std::unordered_map<left_type, std::vector<set_type>> const& selectSets = m_GrammarSetCalculator->GetSelectSet();
		while (!input.empty())
		{
			if (analyzeStack.empty())
			{
				return false;
			}
			char inputTop = input.top();
			member_type analyzeTop = analyzeStack.top();
			if (std::holds_alternative<char>(analyzeTop))
			{
				if (inputTop == std::get<char>(analyzeTop))
				{
					input.pop();
					analyzeStack.pop();
				}
				else
				{
					return false;
				}
			}
			else
			{
				left_type const& nonTerminal = std::get<left_type>(analyzeTop);
				std::vector<set_type> const& correspondingSelectSets = selectSets.at(nonTerminal);
				bool found = false;
				size_t index = 0;
				for (size_t i = 0; i < correspondingSelectSets.size(); ++i)
				{
					if (correspondingSelectSets[i].count(inputTop) != 0)
					{
						found = true;
						index = i;
						break;
					}
				}
				if (!found)
				{
					return false;
				}
				right_type const& corrspondingProduction = productions.at(nonTerminal)[index];
				analyzeStack.pop();
				for (auto it = corrspondingProduction.rbegin(); it != corrspondingProduction.rend(); ++it)
				{
					analyzeStack.push(*it);
				}
			}
		}
		
		return analyzeStack.empty();
	}
}
