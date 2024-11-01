#pragma once

#include <unordered_map>
#include <memory>

namespace Firefly
{
	struct StateNode
	{
		bool isAccepted;
		std::unordered_map<char, int> stateMap;
	};

	class StateTree
	{
	public: 
	private:
	};

}