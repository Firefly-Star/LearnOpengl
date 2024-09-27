#pragma once

#include <typeindex>
#include <vector>

namespace Firefly
{
	enum class EventType
	{
		None = 0,
		KeyPress, KeyRelease, KeyRepeat, 
		MouseMove, MouseScroll, MousePress, MouseRelease,
		WindowResize, WindowClose
	};

	class EventDispatcher;
	class EventManager;

#define EVENT_TYPE_FUNC(x)\
virtual EventType GetType() { return EventType::x; }\
static EventType GetStaticType() { return EventType::x; }

	class Event
	{
		friend class EventDispatcher;
		friend class EventManager;
	public:
		virtual ~Event() = default;
		EVENT_TYPE_FUNC(None);
	protected:
		Event() : m_IsHandled(false) {};
		bool m_IsHandled;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& e) : m_Event(e) {}

		template<typename T>
		bool Dispatch(bool(*func)(T&))
		{
			if (m_Event.GetType() == T::GetStaticType())
			{
				m_Event.m_IsHandled = func(*dynamic_cast<T*>(&m_Event));
				return m_Event.m_IsHandled;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

}