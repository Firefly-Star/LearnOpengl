#pragma once
#include "Event.h"

namespace Firefly
{
	class  MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(double offset) : m_Offset(offset) {}
		double GetOffset() { return m_Offset; }
		EVENT_TYPE_FUNC(MouseScroll);
	private:
		double m_Offset;
	};
}