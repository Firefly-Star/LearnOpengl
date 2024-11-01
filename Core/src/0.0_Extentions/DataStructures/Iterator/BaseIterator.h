#pragma once

#include <iterator>
#include "Range.h"
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
		using diffrence_type = void;
		using reference = void;
	public:
		constexpr explicit BackInsertIterator(Container& container)
			:m_Container(&container)
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
	concept Has_Push_Front = requires(Container container, typename Container::value_type val) { container.push_front(val); };

	template<Has_Push_Front Container>
	class FrontInsertIterator
	{
	public:
		using iterator_strategy = OutputIteratorTag;
		using value_type = void;
		using diffrence_type = void;
		using pointer = void;
		using reference = void;
	public:
		constexpr explicit FrontInsertIterator(Container& container)
			:m_Container(&container)
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

	template<Has_Push_Front Container>
	constexpr FrontInsertIterator<Container> FrontInserter(Container& container) noexcept
	{
		return FrontInsertIterator(container);
	}

	// InsertIterator
	template<typename Container>
	concept Has_Insert = requires(Container container, typename Container::value_type val) { container.insert(val); };

	template<Has_Insert Container>
	class InsertIterator
	{
	public:
		using iterator_strategy = OutputIteratorTag;
		using value_type = void;
		using diffrence_type = void;
		using pointer = void;
		using reference = void;
		using iterator_type = typename Range::Cpo::iterator_t<Container>;
	public:
		constexpr InsertIterator(Container& container, iterator_type position)
			:m_Container(&container), m_Iterator(std::move(position))
		{
		}
		constexpr InsertIterator& operator=(typename Container::ValueType const& val)
		{
			m_Iterator = m_Container->insert(m_Iterator, val);
			++m_Iterator;
			return *this;
		}
		constexpr InsertIterator& operator=(typename Container::ValueType&& val)
		{
			m_Iterator = m_Container->insert(m_Iterator, std::move(val));
			++m_Iterator;
			return *this;
		}
		constexpr InsertIterator& operator*() noexcept
		{
			return *this;
		}
		constexpr InsertIterator& operator++() noexcept
		{
			return *this;
		}
		constexpr InsertIterator operator++(int) noexcept
		{
			return *this;
		}
	private:
		Container* m_Container;
		iterator_type m_Iterator;
	};

	template<Has_Insert Container>
	constexpr InsertIterator<Container> Inserter(Container& container, typename Container::value_type val)
	{
		return InsertIterator(container, val);
	}
}