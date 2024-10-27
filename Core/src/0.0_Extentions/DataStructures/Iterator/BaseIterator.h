#pragma once

#include <iterator>
namespace Firefly
{
	struct InputIteratorTag {};
	struct OutputIteratorTag {};
	struct ForwardIteratorTag : InputIteratorTag{};
	struct BidirectionalIteratorTag : ForwardIteratorTag {};
	struct RandomAccessIteratorTag : BidirectionalIteratorTag {};


	// BackInsertIterator
	template<typename Container>
	concept Has_Push_Back = requires(Container container, typename Container::value_type val) { container.push_back(val); };

	template<Has_Push_Back Container>
	class BackInsertIterator
	{
	public:
		using iterator_catogory = OutputIteratorTag;
		using value_type = void;
		using pointer = void;
		using reference = void;
	public:
		constexpr explicit BackInsertIterator(Container& container)
			:m_Container(container)
		{
		}
		constexpr BackInsertIterator& operator=(typename Container::value_type const& val)
		{
			m_Container->push_back(val);
			return *this;
		}
		constexpr BackInsertIterator& operator=(typename Container::value_type&& val)
		{
			m_Container->push_back(std::move(val));
			return *this;
		}
		constexpr BackInsertIterator& operator*() noexcept
		{
			return *this;
		}
		constexpr BackInsertIterator& operator++() noexcept
		{
			return *this;
		}
		constexpr BackInsertIterator operator++(int) noexcept
		{
			return *this;
		}
	protected:
		Container* m_Container;
	};

	template<Has_Push_Back Container>
	constexpr BackInsertIterator<Container> BackInserter(Container& container) noexcept
	{
		return BackInsertIterator(container);
	}

	// FrontInsertIterator
	template<typename Container>
	concept Has_Push_Back = requires(Container container, typename Container::value_type val) { container.push_back(val); };

	template<typename Container>
	class FrontInsertIterator
	{
	public:
		using IteratrCatogory = OutputIteratorTag;
		using ValueType = void;
		using Pointer = void;
		using Reference = void;
	public:
		constexpr explicit FrontInsertIterator(Container& container)
			:m_Container(container)
		{
		}
		constexpr FrontInsertIterator& operator=(typename Container::ValueType const& val)
		{
			m_Container->push_front(val);
			return *this;
		}
		constexpr FrontInsertIterator& operator=(typename Container::ValueType&& val)
		{
			m_Container->push_front(std::move(val));
			return *this;
		}
		constexpr FrontInsertIterator& operator*() noexcept
		{
			return *this;
		}
		constexpr FrontInsertIterator& operator++() noexcept
		{
			return *this;
		}
		constexpr FrontInsertIterator operator++(int) noexcept
		{
			return *this;
		}
	protected:
		Container* m_Container;
	};

	template<typename Container>
	constexpr FrontInsertIterator<Container> FrontInserter(Container& container) noexcept
	{
		return FrontInsertIterator(container);
	}

}