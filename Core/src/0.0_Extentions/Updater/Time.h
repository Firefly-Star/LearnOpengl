#pragma once

#include "Macros.h"

#include <memory>
#include <mutex>

namespace Firefly
{
	class Time
	{
		SINGLETON(Time);
	public:
		void Update();
		float GetTime() const { return m_Time; }
		float GetDeltaTime() const { return m_Deltatime; }
	private:
		float m_Time;
		float m_Deltatime;
	private:
		Time();
		~Time() = default;
	};
}