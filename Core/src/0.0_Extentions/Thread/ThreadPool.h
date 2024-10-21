#pragma once

#include <vector>
#include <thread>
#include <future>


namespace Firefly
{
	class ThreadPool
	{
	public:
		ThreadPool(unsigned int size);
		~ThreadPool();

		template<typename Func, typename... Args>
		auto submit(Func f, Args&&... args) -> std::future<decltype(f(args...))>;
	private:
		std::vector<std::thread> m_Treads;
		
	};
}