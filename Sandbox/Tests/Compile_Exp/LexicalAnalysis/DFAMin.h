#pragma once
#include "NFA.h"

#include <unordered_set>

namespace Firefly
{
	class DFAMin
	{
	public:
		using state_set = std::unordered_set<StateNode*>;
	public:
		DFAMin() = default;
		~DFAMin() = default;

		[[nodiscard]] StateGraph Minimize(StateGraph dfa);

	private:
		temp<StateNode> m_Allocator;
	};
}