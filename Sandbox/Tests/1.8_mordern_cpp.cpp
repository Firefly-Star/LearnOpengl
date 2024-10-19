#include "Entrypoint.h"
#if CHECK_ACTIVE(1, 8, 1)

#include <type_traits>

struct dummy
{
	int var;
};

decltype(auto) func_0(dummy& d)
{
	return *&d.var;
}

decltype(auto) func_1(dummy& d)
{
	return (d.var);
}

int main()
{
	dummy d{ 0 };
	static_assert(std::is_same_v<decltype(func_0(d)), decltype(func_1(d))>);
}

#endif