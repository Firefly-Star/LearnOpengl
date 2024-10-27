#include "Entrypoint.h"
#if ACTIVE_LESSON_MAJOR == 1 && ACTIVE_LESSON_MINOR == 3 && ISTEST == 1

#include <vector>
#include <utility>
#include <iostream>

void test(std::vector<int>&& ori)
{
	std::cout << "Right version\n";
	std::cout << ori.size() << "\n";
	std::vector<int> a = ori;
	std::cout << ori.size() << "\n";
}

void test(std::vector<int>& ori)
{
	std::cout << "Left version\n";
	std::cout << ori.size() << "\n";
	std::vector<int> a = ori;
	std::cout << ori.size() << "\n";
}

template<typename T>
void process(T&& ori)
{
	test(std::move(ori));
}

int main()
{
	std::vector<int> a = { 1, 3, 2, 4 };
	std::cout << a.size() << "\n";
	process(a);
	std::cout << a.size() << "\n";
}

#endif