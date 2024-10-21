#include "Entrypoint.h"
#if CHECK_ACTIVE(2, 4, 1)

#include <iostream>
#include <atomic>
#include <thread>


void func(std::atomic<int>& counter)
{
	for (int i = 0; i < 1000; ++i)
	{
		++counter;
	}
}

int main()
{
	std::atomic<int> counter(0);
	std::thread t1(func, std::ref(counter));
	std::thread t2(func, std::ref(counter));

	std::cout << counter.load() << "\n";
	t1.join();
	t2.join();
}

#endif