#include "Entrypoint.h"
#if CHECK_ACTIVE(2, 4, 1)

#include "Benchmark/BenchmarkHelper.h"

void t1()
{
	int x = 0;
}

void t2(int x)
{
	++x;
}


int main(int argc, char** argv)
{
	REGISTER(t1);
	REGISTER(t2, 10);
	Firefly::BenchmarkHelper::RunBenchmark(argc, argv);
}

#endif