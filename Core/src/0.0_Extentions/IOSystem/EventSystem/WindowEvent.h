#pragma once
#include "Event.h"

namespace Firefly
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int width, int height) : m_Width(width), m_Height(height) {}
		int GetHeight() const { return m_Height; }
		int GetWidth() const { return m_Width; }
		EVENT_TYPE_FUNC(WindowResize);
	private:
		int m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		EVENT_TYPE_FUNC(WindowClose);
	};
}