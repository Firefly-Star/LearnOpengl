#pragma once

#include <type_traits>
#include <iostream>
#include <variant>
#include "Macros.h"

namespace Firefly
{
	template<typename T>
	class PoolAllocator;

	template<typename T>
	class PoolAllocatorHandle
	{
	public:
		PoolAllocatorHandle(size_t index = 0, const PoolAllocator<T>* allocator = nullptr) : m_Index(index), m_Allocator(allocator) {}
		~PoolAllocatorHandle() = default;

		size_t GetIndex() const { return m_Index; }

		T* Get() const;
		void Release();
		T& operator*() const;
		T* operator->() const;
		bool operator==(PoolAllocatorHandle const& other) const;
		bool operator!=(PoolAllocatorHandle const& other) const;
	private:
		size_t m_Index;
		const PoolAllocator<T>* m_Allocator;
	};

	template<typename T>
	class PoolAllocator
	{
		MOVE_ONLY(PoolAllocator);
	public:
		using unit_type = std::variant<T, size_t>;
		using pointer_type = PoolAllocatorHandle<T>;
	public:
		PoolAllocator(size_t capability = 8);
		~PoolAllocator();
	public:
		template<typename... Args>
		[[nodiscard]]pointer_type Allocate(Args&&... args);

		void Release(pointer_type const& ptr) const;

		T* GetRawPtr(size_t index) const;
	private:
		std::variant<T, size_t>* m_BaseAddr;
		size_t m_Capability;
	private:
		void Dilatation();
	};
}

// Definition
namespace Firefly
{
	template<typename T>
	inline T* PoolAllocatorHandle<T>::Get() const
	{
		return m_Allocator->GetRawPtr(m_Index);
	}

	template<typename T>
	inline void PoolAllocatorHandle<T>::Release()
	{
		if (m_Index != 0 && m_Allocator != nullptr)
		{
			m_Allocator->Release(*this);
		}
		m_Index = 0;
	}

	template<typename T>
	inline T& PoolAllocatorHandle<T>::operator*() const
	{
		return *Get();
	}
	
	template<typename T>
	inline T* PoolAllocatorHandle<T>::operator->() const
	{
		return Get();
	}
	
	template<typename T>
	inline bool PoolAllocatorHandle<T>::operator==(PoolAllocatorHandle const& other) const
	{
		return (m_Index == other.m_Index) && (&m_Allocator == &other.m_Allocator);
	}
	
	template<typename T>
	inline bool PoolAllocatorHandle<T>::operator!=(PoolAllocatorHandle const& other) const
	{
		return !(*this == other);
	}
	
	template<typename T>
	inline PoolAllocator<T>::PoolAllocator(size_t capability)
		:m_BaseAddr(static_cast<unit_type*>(operator new[](sizeof(unit_type) * capability))), m_Capability(capability)
	{
		for (size_t i = 0; i < capability; ++i)
		{
			m_BaseAddr[i] = size_t(i + 1);
		}
	}
	
	template<typename T>
	inline PoolAllocator<T>::~PoolAllocator()
	{
		for (size_t i = 0; i < m_Capability; ++i)
		{
			m_BaseAddr[i] = size_t(i + 1);
		}
		operator delete[](m_BaseAddr);
	}
	
	template<typename T>
	inline PoolAllocator<T>::PoolAllocator(PoolAllocator<T>&& other) noexcept
		:m_BaseAddr(other.m_BaseAddr), m_Capability(other.m_Capability)
	{
		other.m_BaseAddr = nullptr;
	}

	template<typename T>
	inline PoolAllocator<T>& PoolAllocator<T>::operator=(PoolAllocator<T>&& other) noexcept
	{
		if (this != &other)
		{
			delete[]m_BaseAddr;
			m_BaseAddr = other.m_BaseAddr;
			m_Capability = other.m_Capability;
			other.m_BaseAddr = nullptr;
		}
		return *this;
	}

	template<typename T>
	template<typename... Args>
	inline PoolAllocator<T>::pointer_type PoolAllocator<T>::Allocate(Args&&... args)
	{
		if (std::get<size_t>(m_BaseAddr[0]) == m_Capability)
		{
			Dilatation();
		}

		size_t index = std::get<size_t>(m_BaseAddr[0]);
		std::get<size_t>(m_BaseAddr[0]) = std::get<size_t>(m_BaseAddr[index]);
		m_BaseAddr[index].emplace<T>(std::forward<Args>(args)...);

		return pointer_type(index, this);
	}
	
	template<typename T>
	inline void PoolAllocator<T>::Release(pointer_type const& ptr) const
	{
		size_t index = ptr.GetIndex();
		if (index < m_Capability && std::holds_alternative<T>(m_BaseAddr[index]))
		{
			size_t next = std::get<size_t>(m_BaseAddr[0]);
			m_BaseAddr[index].emplace<size_t>(next);
			std::get<size_t>(m_BaseAddr[0]) = index;
		}
	}
	
	template<typename T>
	inline void PoolAllocator<T>::Dilatation()
	{
		//std::cout << "Delatation\n";
		size_t newCapability = 2 * m_Capability;
		unit_type* newMemBlock = static_cast<unit_type*>(operator new[](newCapability * sizeof(unit_type)));
		memcpy(newMemBlock, m_BaseAddr, m_Capability * sizeof(unit_type));
		for (size_t i = m_Capability; i < newCapability; ++i)
		{
			newMemBlock[i].emplace<size_t>(i + 1);
		}
		operator delete[](m_BaseAddr);
		m_BaseAddr = newMemBlock;
		m_Capability = newCapability;
	}

	template<typename T>
	inline T* PoolAllocator<T>::GetRawPtr(size_t index) const
	{
		if (index < m_Capability)
		{
			return std::get_if<T>(m_BaseAddr + index);
		}
		return nullptr;
	}
}

// Hash callable struct for PoolAllocatorHandle
namespace std
{
	template<typename T>
	struct hash<Firefly::PoolAllocatorHandle<T>>
	{
		size_t operator()(const Firefly::PoolAllocatorHandle<T>& obj) const
		{
			return std::hash<size_t>()(obj.GetIndex());
		}
	};
}