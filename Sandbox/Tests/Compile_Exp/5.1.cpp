#include "Entrypoint.h"
#if CHECK_ACTIVE(5, 1, 1)

#include <iostream>
#include <variant>
#include "0.0_Extentions/DataStructures/Allocator/PoolAllocator.h"
#include "crtdbg.h"
#include "LexicalAnalyzer.h"
#include <unordered_set>

#include "NFA.h"
#include "DFA.h"
#include "DFAMin.h"
using namespace Firefly;
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	LexicalAnalyzer analyzer("a.(b|c).d*");
	StateGraph dfa = analyzer.GetDFA();
	std::cout << std::boolalpha << analyzer.Analyze("acddd") << "\n";
	std::cout << std::boolalpha << analyzer.Analyze("ab") << "\n";
	std::cout << std::boolalpha << analyzer.Analyze("addd") << "\n";

	::Firefly::Display(dfa, "C:/Users/Summer/Desktop/comex/aaa.txt");
	std::system("C:/Users/Summer/Desktop/comex/display.bat");

}

#endif