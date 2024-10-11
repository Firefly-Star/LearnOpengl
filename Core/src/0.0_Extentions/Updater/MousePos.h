#pragma once
#include "Macros.h"

#include <memory>
#include <mutex>

namespace Firefly
{
	class MousePos
	{
		SINGLETON(MousePos);
	public:
		void Update();
		float GetX() const { return m_X; }
		float GetY() const { return m_Y; }
		float GetDeltaX() const { return m_DeltaX; }
		float GetDeltaY() const { return m_DeltaY; }
	private:
		float m_X, m_Y, m_DeltaX, m_DeltaY;
		bool m_FirstUpdate;
	private:
		MousePos();
		~MousePos() = default;
	};
}