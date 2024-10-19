#pragma once

#include "Macros.h"

#include <memory>
#include <mutex>

namespace Firefly
{
	class KeyPressEvent;

	class MouseMode
	{
		SINGLETON(MouseMode);
	public:
		void SetKey(int key);
	private:
		int m_Key;
		bool m_Hide;
	private:
		MouseMode(int key = 0);
		~MouseMode();

		bool OnEvent(KeyPressEvent& e);
	};
}