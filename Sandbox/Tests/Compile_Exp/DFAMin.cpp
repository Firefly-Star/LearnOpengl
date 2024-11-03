#include "DFAMin.h"

namespace Firefly
{
    static char alphabet[] = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
        'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };

    StateGraph DFAMin::Minimize(StateGraph dfa)
	{

        std::vector<state_set> partitions;
        state_set acceptingStates, nonAcceptingStates;

        for (auto it = dfa.dfsbegin(); it != dfa.dfsend(); ++it) 
        {
            if ((*it)->isAccepted)
            {
                acceptingStates.insert(*it);
            }
            else 
            {
                nonAcceptingStates.insert(*it);
            }
        }

        if (!acceptingStates.empty())
        {
            partitions.push_back(acceptingStates);
        }
        if (!nonAcceptingStates.empty())
        {
            partitions.push_back(nonAcceptingStates);
        }

        bool changed = true;
        while (changed) 
        {
            changed = false;
            std::vector<state_set> newPartitions;

            for (auto& group : partitions) 
            {
                std::unordered_map<state_set, state_set> splitterMap;
                std::vector<std::pair<state_set, state_set>> splliterMap;
                for (StateNode* state : group) 
                {
                    state_set splitKey;
                    for (char c : alphabet)
                    {
                        StateNode* next = *(state->stateMap[c].begin());
                        for (auto& partition : partitions) 
                        {
                            if (partition.count(next)) 
                            {
                                splitKey.insert(*partition.begin());
                                break;
                            }
                        }
                    }
                    splitterMap[splitKey].insert(state);
                }

                for (auto& entry : splitterMap) 
                {
                    newPartitions.push_back(entry.second);
                    if (entry.second.size() < group.size()) {
                        changed = true;
                    }
                }
            }
            partitions = std::move(newPartitions);
        }

        std::unordered_map<StateNode*, StateNode*> stateMap;
        for (auto& partition : partitions) 
        {
            StateNode* rep = new StateNode();
            rep->isAccepted = (*partition.begin())->isAccepted;
            for (StateNode* state : partition) 
            {
                stateMap[state] = rep;
            }
        }

        for (auto& [oldState, newState] : stateMap)
        {
            for (auto& [c, target] : oldState->transitions) 
            {
                newState->transitions[c] = stateMap[target];
            }
        }

        return StateGraph(stateMap[dfa.getRoot()]);
	}
}
