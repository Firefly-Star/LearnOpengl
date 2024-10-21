#pragma once

#include <condition_variable>
#include <mutex>
#include <semaphore>

#include "Macros.h"

namespace Firefly
{
	class Semaphore
	{
	public:
		Semaphore(Semaphore&&) = delete;
		Semaphore(const Semaphore&) = delete;
		Semaphore& operator=(Semaphore&&) = delete;
		Semaphore& operator=(const Semaphore&) = delete;
	public:
		Semaphore(int initial_count = 1);
		~Semaphore();
		void wait(int need = 1);
		void signal(int release = 1);
	private:
		std::mutex m_Mutex;
		std::unique_lock<std::mutex> m_Lock;
		std::condition_variable m_CV;
		int m_Count;
	};
}