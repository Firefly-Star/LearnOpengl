#pragma once
#include "Event.h"

namespace Firefly
{
	class KeyPressEvent : public Event
	{
	public:
		KeyPressEvent(int key, int mods) : m_Key(key), m_Mods(mods) {}
		virtual ~KeyPressEvent() {};
		int GetKey() { return m_Key; }
		int GetMods() { return m_Mods; }
		EVENT_TYPE_FUNC(KeyPress);
	private:
		int m_Key, m_Mods;
	};

	class KeyReleaseEvent : public Event
	{
	public:
		KeyReleaseEvent(int key) : m_Key(key) {}
		virtual ~KeyReleaseEvent() {}
		int GetKey() { return m_Key; }
		EVENT_TYPE_FUNC(KeyRelease);
	private:
		int m_Key;
	};

	class KeyRepeatEvent : public Event
	{
	public:
		KeyRepeatEvent(int key) : m_Key(key) {}
		virtual ~KeyRepeatEvent() {};
		int GetKey() { return m_Key; }
		EVENT_TYPE_FUNC(KeyRepeat);
	private:
		int m_Key;
	};
}