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
	p.AddProduction("A", {});
	p.AddProduction("B", {'b'});
	p.PrintProductions();
	GrammarAnalyzer analyzer(p);

	std::cout << std::boolalpha << analyzer.CheckIfLL1() << "\n";
	std::cout << "\n";
	std::cout << std::boolalpha << analyzer.Analyze("aaab") << "\n";
	std::cout << std::boolalpha << analyzer.Analyze("c") << "\n";
	std::cout << std::boolalpha << analyzer.Analyze("aaabb") << "\n";
	std::cout << std::boolalpha << analyzer.Analyze("ab") << "\n";
	std::cout << std::boolalpha << analyzer.Analyze("b") << "\n";
}

#endif