#include "Entrypoint.h"
#if CHECK_ACTIVE(1, 8, 1)

#include <type_traits>
#include <xmemory>
#include <stdexcept>
#include <type_traits>
#include <iostream>
struct dummy
{
	int var;
};

[[noreturn]]decltype(auto) func_0(dummy& d)
{
	throw std::runtime_error("adf");
	return *&(d.var);
}

decltype(auto) func_1(dummy& d)
{
	return (d.var);
}

struct foo
{
	void fooo()
	{
		std::cout << "fooo\n";
	}
};

int main()
{
	static_assert(std::is_same_v<
		decltype(func_0(std::declval<dummy&>())),
		decltype(func_1(std::declval<dummy&>()))
	>, "not the same type");
	bool x = noexcept(func_0(std::declval<dummy&>()));
	size_t i = std::_New_alignof<foo>;
	std::cout << (x ? "True" : "False");
	std::cout << i;
#ifdef _MSC_VER
	std::cout << "This is MSVC compiler, version: " << _MSC_VER << std::endl;

#if _MSC_VER >= 1920 // VS 2019
	std::cout << "Using VS 2019 or newer." << std::endl;
#else
	std::cout << "Using an older version of MSVC." << std::endl;
#endif

#else
	std::cout << "This is not MSVC compiler." << std::endl;
#endif
	std::cout << sizeof(void*) << "\n";
	return 0;
}

#endif