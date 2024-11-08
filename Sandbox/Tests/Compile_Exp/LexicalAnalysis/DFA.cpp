#include "DFA.h"

namespace Firefly
{
	auto DFABuilder::AccessibleOfSet(state_set set, char c) -> std::pair<state_set, bool>
	{
		state_set result;
		bool isAccepted = false;
		for (StateNode* node : set)
		{
			if (c == '\0')
			{
				for (auto it = DFSIterator(node); it != DFSIterator(nullptr); it.next({ '\0' }))
				{
					result.insert(*it);
					if ((*it)->isAccepted)
					{
						isAccepted = true;
					}
				}
			}
			else
			{
				state_set epsilonClosure = AccessibleOfSet({ node }, '\0').first;
				state_set temp;
				for (StateNode* eNode : epsilonClosure)
				{
					for (StateNode* cNode : eNode->stateMap[c])
					{
						temp.insert(cNode);
					}
				}
				for (StateNode* cNode : temp)
				{
					for (StateNode* tNode : AccessibleOfSet({ cNode }, '\0').first)
					{
						result.insert(tNode);
						if (tNode->isAccepted)
						{
							isAccepted = true;
						}
					}
				}
			}
		}
		return { result, isAccepted };
	}

	struct Hash 
	{
		template <typename T>
		std::size_t operator()(const std::unordered_set<T>& s) const 
		{
			std::size_t hash = 0;
			for (const auto& elem : s) {
				hash ^= std::hash<T>()(elem) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			}
			return hash;
		}
	};

	struct Equal 
	{
		template <typename T>
		bool operator()(const std::unordered_set<T>& lhs, const std::unordered_set<T>& rhs) const 
		{
			return lhs == rhs;
		}
	};

	static char alphabet[] = {
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
		'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
		'w', 'x', 'y', 'z',
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
	};

	StateNode* DFABuilder::findOrInsertStateSet(std::vector<std::pair<state_set, StateNode*>>& stateSets,
		const state_set& targetSet, bool isAccepted) 
	{
		for (auto& pair : stateSets) 
		{
			if (pair.first == targetSet) 
			{
				return pair.second; 
			}
		}

		StateNode* newNode = m_Allocator.Allocate();
		newNode->isAccepted = isAccepted;
		stateSets.emplace_back(targetSet, newNode);
		return newNode;
	}

	StateGraph DFABuilder::Transform(StateGraph nfa) 
	{
		std::vector<std::pair<state_set, StateNode*>> stateSets;

		StateNode* root = m_Allocator.Allocate();
		{
			auto [stateSet, isAccepted] = AccessibleOfSet({ nfa.GetRoot() }, '\0');
			root->isAccepted = isAccepted;
			stateSets.emplace_back(stateSet, root);
		}

		for (int workIndex = 0; workIndex < stateSets.size(); ++workIndex) 
		{
			state_set currentStateSet = stateSets[workIndex].first;
			StateNode* currentNode = stateSets[workIndex].second;

			for (char c : alphabet) 
			{
				auto [transitionedStateSet, isAccepted] = AccessibleOfSet(currentStateSet, c);
				if (!transitionedStateSet.empty()) 
				{
					StateNode* targetNode = findOrInsertStateSet(stateSets, transitionedStateSet, isAccepted);
					currentNode->stateMap[c].insert(targetNode);
				}
			}
		}

		return StateGraph(root);
	}
}