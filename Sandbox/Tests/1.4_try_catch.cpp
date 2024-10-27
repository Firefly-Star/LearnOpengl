#include "Entrypoint.h"
#if ACTIVE_LESSON_MAJOR == 1 && ACTIVE_LESSON_MINOR == 4 && ISTEST == 1

#include <chrono>
#include <iostream>
#include <thread>
#include "Core.h"

enum class A
{
	None = 0,
	a, b, c
};

int print(A x)
{
	switch (x)
	{
	case A::a: return 1;
	case A::b: return 2;
	case A::c: return 3;
	}
	//return 0;
	throw std::runtime_error("x should not be none");
}

int print2(A x)
{
	switch (x)
	{
	case A::a: return 1;
	case A::b: return 2;
	case A::c: return 3;
	}
	return 0;
}

int m[100000];

void f(A x)
{
	for (int i = 0; i <= 100000; ++i)
	{
		m[i] = print(x);
	}
}

void g(A x)
{
	for (int i = 0; i <= 100000; ++i)
	{
		try
		{
			m[i] = print(x);
		}
		catch (std::runtime_error e)
		{
			m[i] = 0;
		}
	}
}

void h(A x)
{
	for (int i = 0; i <= 100000; ++i)
	{
		try
		{
			m[i] = print2(x);
		}
		catch (std::runtime_error e)
		{
			m[i] = 0;
		}
	}
}

int main()
{
	Firefly::Benchmark test1("test1: function without try-catch");
	Firefly::Benchmark test2("test2: function with try-catch but catch nothing");
	Firefly::Benchmark test3("test3: function with try-catch and catch something");
	Firefly::Benchmark test4("test4: function using return instead of try-catch");

	std::thread t1([&]() {
		test1.Begin();
		f(A::a);
		test1.End();
		test1.Print();
		});

	std::thread t2([&]() {
		test2.Begin();
		g(A::b);
		test2.End();
		test2.Print();
		});
	
	std::thread t3([&]() {
		test3.Begin();
		g(A::None);
		test3.End();
		test3.Print();
		});

	std::thread t4([&]() {
		test4.Begin();
		h(A::None);
		test4.End();
		test4.Print();
		});

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	return 0;
}

#endif