#include "Entrypoint.h"
#if CHECK_ACTIVE(5, 1, 1)

#include <iostream>
#include <variant>
#include "0.0_Extentions/DataStructures/Allocator/PoolAllocator.h"
#include "crtdbg.h"
#include "NFA.h"
#include "DFA.h"
#include <unordered_set>
using namespace Firefly;
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	NFABuilder builder;
	auto graph = builder.Build("(a.b|c)*");
	DFABuilder dfabuilder;
	auto dfa = dfabuilder.Transform(graph);
	Display(dfa, "C:/Users/Summer/Desktop/comex/aaa.txt");
	std::system("C:/Users/Summer/Desktop/comex/display.bat");
}

#endif