#include "Entrypoint.h"
#if CHECK_ACTIVE(1, 5, 1)

#include <iostream>

#define CONCAT(a, b) a##b
#define HELP(i) std::cout << offsetof(T, CONCAT(a, i)) << "\n";

template<size_t count, typename T>
void print()
{
	if constexpr(count > 0)		HELP(0);
	if constexpr(count > 1)		HELP(1);
	if constexpr(count > 2)		HELP(2);
	if constexpr(count > 3)		HELP(3);
	if constexpr(count > 4)		HELP(4);
	if constexpr(count > 5)		HELP(5);
	if constexpr(count > 6)		HELP(6);
	if constexpr(count > 7)		HELP(7);
	if constexpr(count > 8)		HELP(8);
	if constexpr(count > 9)		HELP(9);
	if constexpr(count > 10)	HELP(10);
	if constexpr(count > 11)	HELP(11);
	if constexpr(count > 12)	HELP(12);
}

struct test
{
	float a0[3]; // 4 * 3, 4, 0-11
	char a1; // 1, 1, 12
	double a2[4]; // 8 * 4, 8, 16-47
	bool a3; // 1, 1, 48
	long long a4; // 8, 8, 56-63
};

int main()
{
	print<4, test>();
}

#undef HELP

#endif