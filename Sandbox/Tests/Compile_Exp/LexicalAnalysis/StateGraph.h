#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <memory>
#include <queue>
#include <fstream>

#include "0.0_Extentions/DataStructures/Allocator/PoolAllocator.h"
#include "0.0_Extentions/DataStructures/Iterator/BaseIterator.h"
#include "0.0_Extentions/DataStructures/Algorithm/Graph.h"

namespace Firefly
{
	struct StateNode
	{
		bool isAccepted = false;
		std::unordered_map<char, std::unordered_set<StateNode*>> stateMap;
	};

	class DFSIterator
	{
	public:
		using iterator_category = ForwardIteratorTag;
		using value_type = StateNode*;

		DFSIterator(StateNode* root)
		{
			if (root != nullptr)
			{
				stack.push(root);
				visited.insert(root);
			}
		}

		value_type& operator*()
		{
			if (stack.empty()) {
				throw std::runtime_error("Dereferencing end iterator");
			}
			return stack.top();
		}

		DFSIterator& operator++()
		{
			if (stack.empty()) return *this;

			value_type currentNode = stack.top();
			stack.pop();

			for (auto& pair : currentNode->stateMap)
			{
				for (auto neighbor : pair.second)
				{
					if (visited.insert(neighbor).second)
					{
						stack.push(neighbor);
					}
				}
			}

			return *this;
		}

		DFSIterator operator++(int)
		{
			DFSIterator temp = *this;
			++(*this);
			return temp;
		}

		void next(std::unordered_set<char> set)
		{
			if (stack.empty()) return;

			value_type currentNode = stack.top();
			stack.pop();

			for (char c : set)
			{
				auto it = currentNode->stateMap.find(c);

				if (it != currentNode->stateMap.end())
				{
					for (auto neighbor : it->second)
					{
						if (visited.insert(neighbor).second)
						{
							stack.push(neighbor);
						}
					}
				}
			}
			
			return;
		}

		bool operator!=(const DFSIterator& other)
		{
			return !stack.empty() || !other.stack.empty();
		}
	private:
		std::unordered_set<value_type> visited;
		std::stack<value_type> stack;
	};

	class BFSIterator
	{
	public:
		using value_type = StateNode*;
		BFSIterator(value_type root)
		{
			if (root != nullptr)
			{
				queue.push(root);
				visited.insert(root);
			}
		}

		value_type& operator*()
		{
			if (queue.empty())
			{
				throw std::runtime_error("Dereferencing end iterator");
			}

			return queue.front();
		}

		BFSIterator& operator++()
		{
			if (queue.empty()) return *this;

			value_type& currentNode = queue.front();
			queue.pop();
			visited.insert(currentNode);

			for (auto& pair : currentNode->stateMap)
			{
				for (auto& neighbor : pair.second)
				{
					if (visited.count(neighbor) == 0)
					{
						queue.push(neighbor);
						visited.insert(neighbor);
					}
				}
			}

			return *this;
		}

		BFSIterator operator++(int)
		{
			BFSIterator temp = *this;
			++(*this);
			return temp;
		}

		bool operator!=(BFSIterator const& other)
		{
			return !queue.empty() || !other.queue.empty();
		}

	private:
		std::unordered_set<value_type> visited;
		std::queue<value_type> queue;
	};
	
	class StateGraph
	{
	public:
		using value_type = StateNode*;
		using dfs_iterator_type = DFSIterator;
		using bfs_iterator_type = BFSIterator;
		using allocator_type = PoolAllocator<StateNode>;

		StateGraph(value_type root) : m_Root(root){}

		value_type const& GetRoot() { return m_Root; }

		dfs_iterator_type dfsbegin()
		{
			return dfs_iterator_type(m_Root);
		}

		dfs_iterator_type dfsend()
		{
			return dfs_iterator_type(nullptr);
		}

		bfs_iterator_type bfsbegin()
		{
			return bfs_iterator_type(m_Root);
		}

		bfs_iterator_type bfsend()
		{
			return bfs_iterator_type(nullptr);
		}

	private:
		StateNode* m_Root;
	};

	inline void Display(StateGraph& g, const std::string& filename) {
		std::ofstream dotFile(filename);
		if (!dotFile.is_open()) 
		{
			throw std::runtime_error("Failed to open file for writing: " + filename);
		}
		dotFile << "digraph StateGraph {\n";
		std::unordered_map<StateNode*, int> map;
		int id = 0;
		for (auto it = g.dfsbegin(); it != g.dfsend(); ++it) 
		{
			auto node = *it;
			if (map.find(node) == map.end())
			{
				map[node] = id;
				++id;
			}
			if (node->isAccepted) 
			{
				dotFile << "    " << map[node] << " [shape=doublecircle];\n"; //½ÓÊÜ×´Ì¬
			}
			else 
			{
				dotFile << "    " << map[node] << ";\n";
			}

			for (const auto& pair : node->stateMap) {
				char symbol = pair.first;
				for (StateNode* neighbor : pair.second) {
					if (map.find(neighbor) == map.end())
					{
						map[neighbor] = id;
						++id;
					}
					if (symbol == '\0')
					{
						dotFile << "    " << map[node] << " -> " << map[neighbor] << " [label=\"epsilon\"];\n";
					}
					else
					{
						dotFile << "    " << map[node] << " -> " << map[neighbor] << " [label=" << symbol << "];\n";
					}
				}
			}
		}

		dotFile << "}\n";
		dotFile.close();
	}

}