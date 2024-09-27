#pragma once
#include "1.3_Hello_Window/Window.h"

namespace Firefly
{
	class MousePos
	{
	public:
		static void Update() 
		{
			if (s_FirstUpdate)
			{
				double x, y;
				Window::GetInstance().GetCursorPos(&x, &y);
				float fx = static_cast<float>(x);
				float fy = static_cast<float>(y);
				s_X = fx;
				s_Y = fy;
				s_FirstUpdate = false;
			}
			else
			{
				double x, y;
				Window::GetInstance().GetCursorPos(&x, &y);
				float fx = static_cast<float>(x);
				float fy = static_cast<float>(y);
				s_DeltaX = fx - s_X;
				s_DeltaY = fy - s_Y;
				s_X = fx;
				s_Y = fy;
			}
		}
		static float GetX() { return s_X; }
		static float GetY() { return s_Y; }
		static float GetDeltaX() { return s_DeltaX; }
		static float GetDeltaY() { return s_DeltaY; }
	private:
		static float s_X, s_Y, s_DeltaX, s_DeltaY;
		static bool s_FirstUpdate;
	};
}