#include "GlobalObjectManager.h"

namespace Firefly
{
#if 0
	GlobalObjectManager& GlobalObjectManager::GetInstance()
	{
		static GlobalObjectManager instance;
		return instance;
	}

	void GlobalObjectManager::Remove(unsigned int index)
	{
		auto it = m_Table.find(index);
		if (it != m_Table.end())
		{
			it->second.reset(nullptr);
		}
		else
		{
			throw std::runtime_error("Invalid index: " + std::to_string(index));
		}
	}
#endif
}