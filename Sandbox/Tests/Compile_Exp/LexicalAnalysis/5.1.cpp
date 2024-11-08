#include "Entrypoint.h"
#if CHECK_ACTIVE(5, 1, 1)

#include <iostream>
#include "crtdbg.h"
#include "LexicalAnalyzer.h"
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Firefly::LexicalAnalyzer analyzer("a*.d*|e.(a.b.a)*.a");
	Firefly::NFABuilder builder;
	Firefly::StateGraph nfa = builder.Build("a*.d*|e.(a.b.a)*.a");
	Firefly::DFABuilder dfabuilder;
	Firefly::StateGraph dfa = dfabuilder.Transform(nfa);

	/*Firefly::StateGraph dfa = analyzer.GetDFA();
	std::cout << std::boolalpha << analyzer.Analyze("eabaabaa") << "\n";
	std::cout << std::boolalpha << analyzer.Analyze("eabba") << "\n";
	std::cout << std::boolalpha << analyzer.Analyze("addd") << "\n";*/

	Firefly::Display(dfa, "C:/Users/Summer/Desktop/comex/aaa.txt");
	std::system("C:/Users/Summer/Desktop/comex/display.bat");
}

#endif