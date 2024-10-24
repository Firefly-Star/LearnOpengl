#pragma once

#include <chrono>
#include <iostream>
#include <string>
#include <mutex>
namespace Firefly
{
	class Benchmark
	{
	public:
		Benchmark(const std::string& description = "No decription provided") : m_Description(description), m_Duration(0) {}
		void Reset() { m_Duration = 0; }
		void Begin() { m_Begin = std::chrono::steady_clock::now(); }
		void End()
		{
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			m_Duration = static_cast<unsigned int>(std::chrono::duration_cast<std::chrono::microseconds>(end - m_Begin).count());
		}
		unsigned int GetCount() { return m_Duration; }
		void Print() 
		{
			s_Mutex.lock();
			std::cout << m_Description << " takes " << m_Duration / 1000 << "." << m_Duration % 1000 << " milliseconds.\n";
			s_Mutex.unlock();
		}
	private:
		std::string m_Description;
		unsigned int m_Duration;
		std::chrono::steady_clock::time_point m_Begin;

		static std::mutex s_Mutex;
	};
}