#include "Semaphore.h"

namespace Firefly
{
	Semaphore::Semaphore(int initial_count)
		:m_Count(initial_count), m_Mutex(), m_Lock(m_Mutex, std::defer_lock), m_CV()
	{
	}

	Semaphore::~Semaphore()
	{
	}

	void Semaphore::wait(int need)
	{
		m_Lock.lock();
		m_Count -= need;
		m_CV.wait(m_Lock, [&]() {
			return m_Count < 0;
			});
		m_Lock.unlock();
	}
	
	void Semaphore::signal(int release)
	{
		m_Lock.lock();
		m_Count += release;
		
		if (m_Count > 0) {
			m_CV.notify_all();
		}
		m_Lock.unlock();
	}
}