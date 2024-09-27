#pragma once
#include "MousePos.h"
#include "Time.h"
namespace Firefly
{
	class Updater
	{
	public:
		static void Update()
		{
			MousePos::Update();
			Time::Update();
		}
	};
}