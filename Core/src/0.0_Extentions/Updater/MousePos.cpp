#include "MousePos.h"
#include "1.3_Hello_Window/Window.h"

namespace Firefly
{
	SINGLETON_DEFINATION(MousePos);
	void MousePos::Update()
	{
		if (m_FirstUpdate)
		{
			double x, y;
			Window::GetInstance().GetCursorPos(&x, &y);
			m_X = static_cast<float>(x);
			m_Y = static_cast<float>(y);
			m_FirstUpdate = false;
		}
		else
		{
			double x, y;
			Window::GetInstance().GetCursorPos(&x, &y);
			m_DeltaX = static_cast<float>(x) - m_X;
			m_DeltaY = static_cast<float>(y) - m_Y;
			m_X = static_cast<float>(x);
			m_Y = static_cast<float>(y);
		}
	}

	MousePos::MousePos()
		:m_X(0.0f), m_Y(0.0f), m_DeltaX(0.0f), m_DeltaY(0.0f)
	{
	}
}