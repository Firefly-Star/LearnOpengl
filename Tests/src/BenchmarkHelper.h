#pragma once

#include <string>
#include <utility>
#include <benchmark/benchmark.h>

namespace Firefly
{
	class BenchmarkHelper
	{
	public:
		template<typename Func, typename... Args>
		static void RegisterBenchmark(const char* name, Func func, Args&&... args);

		static void RunBenchmark(int argc = 0, char** argv = nullptr);
	};

	template<typename Func, typename ...Args>
	inline void BenchmarkHelper::RegisterBenchmark(const char* name, Func func, Args&&...args)
	{
		if constexpr (sizeof...(Args) == 0)
		{
			benchmark::RegisterBenchmark(name, [func](benchmark::State& state) {
				for (auto _ : state)
				{
					func();
				}
				});
		}
		else
		{
			auto boundArgs = std::forward_as_tuple(std::forward<Args>(args)...);

			benchmark::RegisterBenchmark(name, [func, boundArgs = std::move(boundArgs)](benchmark::State& state) {
				for (auto _ : state)
				{
					std::apply(func, boundArgs);
				}
				});
		}
	}

#define REGISTE(func, ...) ::benchmark::RegisterBenchmark(#func, func, __VA_ARGS__);
}