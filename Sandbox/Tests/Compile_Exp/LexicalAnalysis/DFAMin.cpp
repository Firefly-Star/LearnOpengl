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
        state_set accepted;
        state_set unaccepted;

        for (auto it = dfa.dfsbegin(); it != dfa.dfsend(); ++it)
        {
            StateNode* node = *it;
            if (node->isAccepted)
            {
                accepted.insert(node);
            }
            else
            {
                unaccepted.insert(node);
            }
        }
        if (!accepted.empty())
        {
            partitions.push_back(accepted);
        }
        partitions.push_back(unaccepted);

        bool changed = true;
        while (changed)
        {
            changed = false;
            std::vector<state_set> newPartitions;
            for (state_set& group : partitions)
            {
                // subdivision
                std::unordered_map<std::string, state_set> transitionMap;
                for (StateNode* state : group)
                {
                    std::string key;

                    for (char c : alphabet)
                    {
                        if (state->stateMap.count(c) != 0)
                        {
                            StateNode* nextState = *(state->stateMap[c].begin());
                            for (size_t i = 0; i < partitions.size(); ++i)
                            {
                                if (partitions[i].count(nextState) > 0)
                                {
                                    key += std::to_string(i) + ",";
                                    break;
                                }
                            }
                        }
                        else
                        {
                            key += "-1,";
                        }
                    }

                    transitionMap[key].insert(state);
                }

                for (const auto& pair : transitionMap)
                {
                    newPartitions.push_back(pair.second);
                    if (pair.second.size() != group.size())
                    {
                        changed = true;
                    }
                }
            }

            partitions = std::move(newPartitions);
        }


        // 构造最小化 DFA
        // 对partitions中的每个分组，对应地创建一个StateNode*，分组中包含了接收结点的就是新的接收结点，包含了起始结点的就是起始结点
        StateNode* root;
        int n = partitions.size();
        std::vector<StateNode*> minimizedDFA(n);
        for (int i = 0; i < n; ++i)
        {
            minimizedDFA[i] = m_Allocator.Allocate();
        }

        for (int i = 0; i < n; ++i)
        {
            for (StateNode* state : partitions[i])
            {
                if (state == dfa.GetRoot())
                {
                    root = minimizedDFA[i];
                }
                if (state->isAccepted)
                {
                    minimizedDFA[i]->isAccepted = true;
                }
            }
            
            StateNode* oneNode = *(partitions[i].begin());
            for (char c : alphabet)
            {
                if (oneNode->stateMap.count(c) != 0)
                {
                    StateNode* nextNode = *(oneNode->stateMap[c].begin());
                    for (int j = 0; j < n; ++j)
                    {
                        if (partitions[j].count(nextNode) != 0)
                        {
                            minimizedDFA[i]->stateMap[c].insert(minimizedDFA[j]);
                            break;
                        }
                    }
                }
            }
        }

        return StateGraph(root);
	}
}
