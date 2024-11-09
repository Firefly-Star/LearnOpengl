#include "Entrypoint.h"
#if CHECK_ACTIVE(5, 2, 1)

#include "GrammarAnalyzer.h"
#include "GrammarNormalizer.h"
#include <iostream>
#include <unordered_set>
#include <set>
#include <string>
#include <variant>

using namespace Firefly;
int main()
{
	Grammar p;
	p.SetStartSymbol("S");
	p.AddProduction("S", { "A", "B" });
	p.AddProduction("A", { 'a', "A" });
	p.AddProduction("A", { });
	p.AddProduction("B", { 'b' });
	p.PrintProductions();
#if 1
	GrammarAnalyzer analyzer(p);

	std::cout << std::boolalpha << analyzer.CheckIfLL1() << "\n";
	std::cout << "\n";
	std::cout << std::boolalpha << analyzer.Analyze("aaab") << "\n";
	std::cout << std::boolalpha << analyzer.Analyze("c") << "\n";
	std::cout << std::boolalpha << analyzer.Analyze("aaabb") << "\n";
	std::cout << std::boolalpha << analyzer.Analyze("ab") << "\n";
	std::cout << std::boolalpha << analyzer.Analyze("b") << "\n";
#endif
#if 0
	std::cout << "\n";
	GrammarNormalizer normalizer(p);
	normalizer.Normalize();
	GrammarSetCalculator calculator(p);
#if 0
	auto const& firstSet = calculator.GetFirstSet();
	auto const& followSet = calculator.GetFollowSet();
	auto const& selectSet = calculator.GetSelectSet();
	std::cout << "\nFirstSet: \n";
	for (auto const& [left, firsts] : firstSet)
	{
		std::cout << left << ": \n";
		for (auto f : firsts)
		{
			std::visit([](auto const& value) {std::cout << value << ", "; }, f);
		}
		std::cout << "\n";
	}
	std::cout << "\nFollowSet: \n";
	for (auto const& [left, follows] : followSet)
	{
		std::cout << left << ": \n";
		for (auto f : follows)
		{
			std::visit([](auto const& value) {std::cout << value << ", "; }, f);
		}
		std::cout << "\n";
	}
	std::cout << "\nSelectSet: \n";
	auto const& productions = p.GetProductions();
	for (auto const& [left, selects] : selectSet)
	{
		for (size_t i = 0; i < selects.size(); ++i)
		{
			std::cout << left << " -> ";
			for (auto const& m : productions.at(left)[i])
			{
				std::visit([](auto const& value) {std::cout << value << " "; }, m);
			}
			std::cout << ":\n";
			for (auto const& m : selects[i])
			{
				std::visit([](auto const& value) {std::cout << value << ", "; }, m);
			}
			std::cout << "\n";
		}
	}
#endif
#endif
}

#endif