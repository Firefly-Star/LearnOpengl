#include "Entrypoint.h"
#if CHECK_ACTIVE(2, 8, 1)

#include <iostream>
#include <crtdbg.h>

#include "Core.h"
#include "BenchmarkHelper.h"
#include "xmemory"

void BM_MyAllocate(benchmark::State& state, size_t size)
{
	for (auto _ : state)
	{
		void* ptr = Firefly::Allocator<>::AllocateAligned<32>(size);
		state.PauseTiming();
		Firefly::Allocator<>::DeAllocate<32>(ptr, size);
		state.ResumeTiming();
	}
}

void BM_STLAllocate(benchmark::State& state, size_t size)
{
	for (auto _ : state)
	{
		void* ptr = std::_Allocate<32>(size);
		state.PauseTiming();
		std::_Deallocate<32>(ptr, size);
		state.ResumeTiming();
	}
}
struct dummy
{
	size_t size;
};

namespace AAA
{
	struct x {};
	void foo(x)
	{
		std::cout << "aaa";
	}
	x x_v;
}

void foo(AAA::x)
{
	std::cout << "xxx";
}

void bar()
{
	::foo(AAA::x_v);
}


#include <xutility>
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	REGISTE(BM_MyAllocate, 32000);
	REGISTE(BM_STLAllocate, 32000);
	//Firefly::BenchmarkHelper::RunBenchmark();
	bar();
	
}
#endif