#include "BenchmarkHelper.h"

#include "benchmark/benchmark.h"
#include <utility>
#include <stdexcept>

namespace Firefly
{
	void BenchmarkHelper::RunBenchmark(int argc, char** argv)
	{
		char arg0_default[] = "benchmark"; char* args_default = arg0_default;
		if (!argv) 
		{
			argc = 1; argv = &args_default;
		}
		::benchmark::Initialize(&argc, argv);

		if (::benchmark::ReportUnrecognizedArguments(argc, argv))
		{
			throw std::runtime_error("");
		}

		::benchmark::RunSpecifiedBenchmarks();

		::benchmark::Shutdown();
	}
}