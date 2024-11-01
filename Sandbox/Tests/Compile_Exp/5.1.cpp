#include "Entrypoint.h"
#if CHECK_ACTIVE(5, 1, 1)

#include <iostream>
#include <variant>
#include "0.0_Extentions/DataStructures/Allocator/PoolAllocator.h"
#include "crtdbg.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	Firefly::PoolAllocator<int> allocator;
	int** ptr = new int* [100];
	for (int i = 0; i < 100; ++i)
	{
		ptr[i] = allocator.Allocate();
	}
	delete[]ptr;
}

#endif