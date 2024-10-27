#include "EntryPoint.h"
#if CHECK_ACTIVE(2, 5, 1)

#include <utility>
#include <tuple>
#include <iostream>
#include <type_traits>
class dummy
{
public:
	dummy() : x(0) {}
	dummy(dummy&& other) = delete;
	dummy& operator=(dummy&& other) = delete;
	
	int x;
};

class dummy2
{
public:
	dummy2() : x(0) {}
	dummy2(dummy2&& other) noexcept
		:x(0)
	{
		std::cout << "Move con.\n";
	}
	dummy2& operator=(dummy2&& other) noexcept
	{
		std::cout << "Move to.\n";
		return *this;
	}

	int x;
};

void print(dummy& a)
{
	std::cout << "l Ref.\n";
}

void print(dummy&& a)
{
	std::cout << "r ref.\n";
}

template<typename ...Args>
void print2(Args&&... args)
{
	(print(std::forward<Args>(args)), ...);
}

void print3(dummy&& x, dummy&& y, dummy&& z)
{
	std::cout << "r.\n";
}

void print3(dummy& x, dummy& y, dummy& z)
{
	std::cout << "l.\n";
}

template<typename... Args>
void X(Args&&... args)
{
	auto boundArgs = std::forward_as_tuple(args...);

	if (std::is_same_v<decltype(boundArgs), std::tuple<dummy&&, dummy&, dummy2&&>>)
	{
		std::cout << "True.\n";
	}

	auto ba2 = std::move(boundArgs);
	if (std::is_same_v<decltype(ba2), std::tuple<dummy&&, dummy&, dummy2&&>>)
	{
		std::cout << "True.\n";
	}

	print((std::get<0>(ba2)));
}

template<typename... Args>
void Y(Args&&... args)
{
	auto boundArgs = std::forward_as_tuple(args...);
	std::apply(print3, boundArgs);
}

template<typename T>
void p(T&& x)
{
	if constexpr (std::is_lvalue_reference_v<T>) {
		std::cout << "l\n"; // x 是左值引用
	}
	else {
		std::cout << "r\n"; // x 是右值引用
	}
}

int main()
{
	/*{
		dummy x1;
		dummy x2;
		dummy2 x3;
		X(std::move(x1), x2, std::move(x3));
	}*/
	{
		dummy x1;
		dummy x2;
		dummy x3;
		auto x = std::forward_as_tuple(std::move(x1), x2, std::move(x3));
		auto y = std::move(x);
		p(std::get<0>(y));
	}
}

#endif