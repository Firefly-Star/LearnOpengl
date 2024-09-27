#include "Entrypoint.h"
#if ACTIVE_LESSON_MAJOR == 1 && ACTIVE_LESSON_MINOR == 4 && TEST == 1

#include <chrono>
#include <iostream>

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
		if (print(x) == 0)
		{
			m[i] = 0;
		}
		else
		{
			m[i] = print(x);
		}

	}
}

int main()
{
	Firefly::Benchmark test1("function without try-catch");
	Firefly::Benchmark test2("function with try-catch but catch nothing");
	Firefly::Benchmark test3("function with tyr-catch and catch something");
	test1.Begin();
	f(A::a);
	test1.End();
	test1.Print();

	test2.Begin();
	g(A::b);
	test2.End();
	test2.Print();

	test3.Begin();
	g(A::None);
	test3.End();
	test3.Print();
}

#endif