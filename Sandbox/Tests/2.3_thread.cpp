#include "Entrypoint.h"
#if CHECK_ACTIVE(2, 3, 1)

#include <iostream>
#include <semaphore>
#include <thread>

std::counting_semaphore<1> ac(0), bc(0), cd(0), ce(0), ef(0);

void t1Func();
void t2Func();

int main()
{
	std::thread T1(t1Func);
	std::thread T2(t2Func);

	T1.join();
	T2.join();
}

void t1Func()
{
	std::cout << "A task\n";
	ac.release();

	ce.acquire();
	std::cout << "E task\n";
	// ef.release();
	
	// ef.acquire();
	std::cout << "F task\n";
}

void t2Func()
{
	std::cout << "B task\n";
	// bc.release();
	 
	// bc.acquire();
	std::cout << "C task\n";
	ce.release();
	// cd.release();
	
	// cd.acquire();
	std::cout << "D task\n";
}

#endif