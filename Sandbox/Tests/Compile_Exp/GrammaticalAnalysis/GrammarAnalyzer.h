#pragma once

#include "Grammar.h"
#include "GrammarSetCalculator.h"
#include <memory>
namespace Firefly
{
	class GrammarAnalyzer
	{
	public:
		using left_type = Grammar::left_type;
		using member_type = Grammar::member_type;
		using right_type = Grammar::right_type;
		using set_type = Grammar::set_type;
	public:
		explicit GrammarAnalyzer(Grammar const& grammar);
		~GrammarAnalyzer() = default;

		bool CheckIfLL1() const;
		bool Analyze(std::string const& sentence) const;

		Grammar const& GetGrammar() const { return m_Grammar; }
		GrammarSetCalculator const& GetSet() const { return *m_GrammarSetCalculator; }
	private:

	private:
		Grammar m_Grammar;
		std::unique_ptr<GrammarSetCalculator> m_GrammarSetCalculator;
	};
}