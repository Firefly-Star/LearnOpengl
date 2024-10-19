#pragma once

#include <functional>

namespace Firefly
{
	class SingletonManager
	{
	public:
		static void Init();
		static void Terminate();
	};
}