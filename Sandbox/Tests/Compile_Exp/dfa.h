#pragma once

#include "NFA.h"

namespace Firefly
{
	class DFABuilder
	{
	public:
		using state_set = std::unordered_set<StateNode*>;
	public:
		DFABuilder() = default;
		~DFABuilder() = default;

		std::pair<state_set, bool> AccessibleOfSet(state_set set, char c);

		StateNode* findOrInsertStateSet(std::vector<std::pair<state_set, StateNode*>>& stateSets,
			const state_set& targetSet, bool isAccepted);

		StateGraph Transform(StateGraph nfa);
	private:
		temp<StateNode> m_Allocator;
	};

}