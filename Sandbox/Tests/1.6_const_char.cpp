#include "Entrypoint.h"
#if CHECK_ACTIVE(1, 6, 1)

#include <sstream>
#include <iostream>

void printAddr(const char* name)
{
	std::cout << std::hex << reinterpret_cast<size_t>(name) << "\n";
}

int main()
{
	for (unsigned int i = 0; i < 100; ++i)
	{
		std::stringstream ss;
		ss << "offsets[" << i << "]";
		const char* name = ss.str().c_str();
		printAddr(name);
	}
}

#endif