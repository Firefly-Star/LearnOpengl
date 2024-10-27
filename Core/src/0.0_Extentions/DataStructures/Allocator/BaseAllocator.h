#pragma once

#include <type_traits>
#include "0.0_Extentions/DataStructures/Helper.h"
namespace Firefly
{
	class DefaultAllocatorTrait
	{
	public:
		static void* Allocate(size_t size)
		{
			return ::operator new(size);
		}
		
		template<size_t alignment>
		static std::enable_if_t<IsPowerOf2(alignment), void*>
			AllocateAligned(size_t size)
		{
			return ::operator new(size, std::align_val_t{ alignment });
		}

		template<size_t alignment>
		static std::enable_if_t<IsPowerOf2(alignment), void> 
			DeAllocate(void* ptr, size_t size)
		{
			operator delete(ptr, size, std::align_val_t{ alignment });
		}
	};

	template<class Trait = DefaultAllocatorTrait>
	class Allocator
	{
	public:
		static void* Allocate(size_t size)
		{
			return Trait::Allocate(size);
		}

		template<size_t alignment>
		static std::enable_if_t < IsPowerOf2(alignment), void* >
			AllocateAligned(size_t size)
		{
			return Trait::AllocateAligned<alignment>(size);
		}

		template<size_t alignment>
		static std::enable_if_t<IsPowerOf2(alignment), void>
			DeAllocate(void* ptr, size_t size)
		{
			Trait::DeAllocate<32>(ptr, size);
		}
	};
}