#include "NFA.h"

#include <stack>

namespace Firefly
{
	StateGraph NFABuilder::Build(std::string const& regex)
	{
		std::stack<StateGraph> nfaStack;
		std::stack<char> operatorStack;

		for (auto c : regex)
		{
			if (IsAlpha(c))
			{
				nfaStack.push(SimpleNFA(c));
			}
			else
			{
				switch (c)
				{
				case '(':
				{
					operatorStack.push(c);
					break;
				}
				case ')':
				{
					while (!operatorStack.empty() && operatorStack.top() != '(')
					{
						char op = operatorStack.top();
						operatorStack.pop();
						ApplyOperator(op, nfaStack);
					}
					operatorStack.pop(); // pop '('
					break;
				}
				default:
				{
					while (!operatorStack.empty() && (OperatorPriority(operatorStack.top()) >= (OperatorPriority(c))))
					{
						char op = operatorStack.top();
						operatorStack.pop();
						ApplyOperator(op, nfaStack);
					}
					operatorStack.push(c);
					break;
				}
				}
			}
		}

		while (!operatorStack.empty())
		{
			char op = operatorStack.top(); operatorStack.pop();
			ApplyOperator(op, nfaStack);
		}

		return nfaStack.top();
	}

	int NFABuilder::OperatorPriority(char c)
	{
		switch (c)
		{
		case '*':return 3;
		case '.':return 2;
		case '|':return 1;
		}
		return 0;
	}

	void NFABuilder::ApplyOperator(char op, std::stack<StateGraph>& nfaStack)
	{
		if (op == '*') {
			StateGraph nfa = nfaStack.top(); 
			nfaStack.pop();
			nfaStack.push(Closure(nfa));
		}
		else if (op == '.') {
			StateGraph right = nfaStack.top(); 
			nfaStack.pop();
			StateGraph left = nfaStack.top(); 
			nfaStack.pop();
			nfaStack.push(JoinNFA(left, right));
		}
		else if (op == '|') {
			StateGraph right = nfaStack.top(); 
			nfaStack.pop();
			StateGraph left = nfaStack.top(); 
			nfaStack.pop();
			nfaStack.push(UnionNFA(left, right));
		}
	}


	bool NFABuilder::IsAlpha(char c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
	}

	StateGraph NFABuilder::SimpleNFA(char c) // a
	{
		StateNode* root = m_Allocator.Allocate();
		root->isAccepted = false;
		StateNode* temp = m_Allocator.Allocate();
		temp->isAccepted = true;
		root->stateMap[c].insert(temp);
		return StateGraph(root);
	}

	StateGraph NFABuilder::JoinNFA(StateGraph first, StateGraph second) // R1R2
	{
		std::unordered_set<StateNode*> FAnodes;
		for (auto it = first.dfsbegin(); it != first.dfsend(); ++it)
		{
			auto& node = *it;
			if (node->isAccepted)
			{
				FAnodes.insert(node);
			}
		}
		for (auto node : FAnodes)
		{
			node->isAccepted = false;
			node->stateMap['\0'].insert(second.GetRoot());
		}
		return first;
	}

	StateGraph NFABuilder::UnionNFA(StateGraph first, StateGraph second) // R1 | R2
	{
		StateNode* temp = m_Allocator.Allocate();
		temp->isAccepted = false;

		temp->stateMap['\0'].insert(first.GetRoot());
		temp->stateMap['\0'].insert(second.GetRoot());

		return StateGraph(temp);
	}

	StateGraph NFABuilder::Closure(StateGraph first)
	{
		StateNode* temp = m_Allocator.Allocate();
		temp->isAccepted = true;
		temp->stateMap['\0'].insert(first.GetRoot());
		for (auto it = first.bfsbegin(); it != first.bfsend(); ++it)
		{
			if ((*it)->isAccepted)
			{
				(*it)->stateMap['\0'].insert(first.GetRoot());
			}
		}
		return StateGraph(temp);
	}
}