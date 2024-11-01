#pragma once
#include <vector>
#include <queue>
#include <unordered_map>
#include "0.0_Extentions/DataStructures/Iterator/BaseIterator.h"
#include "0.0_Extentions/DataStructures/Allocator/BaseAllocator.h"
#include <type_traits>
namespace Firefly
{
	std::vector<int> x;
	template<typename Vec>
	class _VectorConstIterator
	{
	public:
		using iterator_strategy = RandomAccessIteratorTag;
		using value_type = typename Vec::value_type;
		using diffrence_type = typename Vec::dirffrence_type;
		using pointer = typename Vec::pointer;
		using reference = typename reference;
	public:

	private:
		pointer
	};

}