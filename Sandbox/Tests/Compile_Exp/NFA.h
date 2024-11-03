#pragma once
#include "StateGraph.h"

namespace Firefly
{
	template<typename T>
	class temp
	{
	public:
		T* Allocate()
		{
			T* node =  new T();
			m_Pool.insert(node);
			return node;
		}

		void Deallocate(T* ptr)
		{
			if (m_Pool.count(ptr) == 0)
			{
				throw std::runtime_error("ptr out of management of allocator.");
			}
			else
			{
				m_Pool.erase(ptr);
			}
		}

		~temp()
		{
			for (auto ptr : m_Pool)
			{
				delete ptr;
			}
		}

	private:
		std::unordered_set<T*> m_Pool;
	};

	class NFABuilder
	{
	public:
		NFABuilder() = default;
		~NFABuilder() = default;

		StateGraph Build(std::string const& regex);
	private:
		int OperatorPriority(char c);

		void ApplyOperator(char op, std::stack<StateGraph>& nfaStack);

		bool IsAlpha(char c);

		StateGraph SimpleNFA(char c); // a
		
		StateGraph JoinNFA(StateGraph first, StateGraph second); // R1R2

		StateGraph UnionNFA(StateGraph first, StateGraph second); // R1 | R2

		StateGraph Closure(StateGraph first);
	private:
		//PoolAllocator<StateNode> m_Allocator;
		temp<StateNode> m_Allocator;
	};
}