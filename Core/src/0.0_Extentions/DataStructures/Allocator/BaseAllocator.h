#pragma once

#include <type_traits>
#include "0.0_Extentions/DataStructures/Helper.h"
namespace Firefly
{
	class _DefaultAllocatorTrait
	{
	public:
		static void* Allocate(size_t size)
		{
			return ::operator new(size);
		}
		
		template<size_t alignment>
		requires Is_PowerOf2<alignment>
		void* AllocateAligned(size_t size)
		{
			return ::operator new(size, std::align_val_t{ alignment });
		}

		template<size_t alignment>
		requires Is_PowerOf2<alignment>
		void DeAllocate(void* ptr, size_t size)
		{
			operator delete(ptr, size, std::align_val_t{ alignment });
		}
	};

	template<class Trait = _DefaultAllocatorTrait>
	class _Allocator
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

	template<typename T>
	class Allocator
	{
	public:
		static_assert(!std::is_const_v<T>, "The C++ Standard forbids containers of const elements "
			"because allocator<const T> is ill-formed.");
		static_assert(!std::is_function_v<T>, "The C++ Standard forbids allocators for function elements "
			"because of [allocator.requirements].");
		static_assert(!std::is_reference_v<T>, "The C++ Standard forbids allocators for reference elements "
			"because of [allocator.requirements].");

		using value_type = T;

	public:

	private:

	};
}