#include "Entrypoint.h"
#if CHECK_ACTIVE(2, 7, 1)

#include <vector>
#include "BenchmarkHelper.h"
#include "vcruntime_new.h"
#include <xmemory>


void BM_VB1(benchmark::State& state, unsigned int size)
{
	for (auto _ : state)
	{
		std::vector<int> v;
		for (int i = 0; i < size; ++i)
		{
			v.push_back(i);
		}
		benchmark::DoNotOptimize(v);
	}
}

void BM_VB2(benchmark::State& state, unsigned int size)
{
	for (auto _ : state)
	{
		std::vector<int> v(size);
		for (int i = 0; i < size; ++i)
		{
			v[i] = i;
		}
		benchmark::DoNotOptimize(v);
	}
}

void BM_VB3(benchmark::State& state, unsigned int size)
{
	for (auto _ : state)
	{
		std::vector<int> v;
		v.reserve(size);
		for (int i = 0; i < size; ++i)
		{
			v.push_back(i);
		}
		benchmark::DoNotOptimize(v);
	}
}

void BM_NEW(benchmark::State& state, size_t size, size_t align)
{
	for (auto _ : state)
	{
		state.ResumeTiming();
		void* ptr = ::operator new(size, std::align_val_t{ align });
		state.PauseTiming();
		operator delete(ptr, size);
	}
}

void BM_NEW2(benchmark::State& state, size_t size, size_t align)
{
	std::_Default_allocate_traits allocator;
	for (auto _ : state)
	{
		state.ResumeTiming();
		void* ptr = allocator._Allocate_aligned(size, align);
		state.PauseTiming();
		operator delete(ptr, size);
	}
}

void BM_NEW3(benchmark::State& state, size_t size, size_t align)
{

}
#define A(x)\
BENCHMARK_CAPTURE(BM_NEW, "operator new" #x, 25600, x);\
BENCHMARK_CAPTURE(BM_NEW2, "_Allocate_aligned" #x, 25600, x)

int main()
{
	/*REGISTE(BM_VB1, 100000);
	REGISTE(BM_VB2, 100000);
	REGISTE(BM_VB3, 100000);*/
	A(2);
	A(4);
	A(8);
	A(16);
	A(32);
	A(64);
	A(128);
	A(256);

	Firefly::BenchmarkHelper::RunBenchmark();
}

#endif