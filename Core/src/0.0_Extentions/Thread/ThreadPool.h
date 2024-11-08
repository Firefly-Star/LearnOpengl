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

		void Start();
		void Shutdown();
		void ForceShutdown();

		template<typename Func, typename... Args>
		void Submit(Func func, Args&&... args);

		void ClearQueue();
		int GetQueueSize() const;

		int GetActiveThreadCount() const;
		int GetTotalThreadCount() const;
		
		void SetMaxThreads(int maxThreads);
		void ResizePool(int newSize);
		
		void CancelTask(int taskId);
		void WaitForAll();

		void Pause();
		void Resume();
	private:
		std::vector<std::thread> m_Treads;
		
	};
}