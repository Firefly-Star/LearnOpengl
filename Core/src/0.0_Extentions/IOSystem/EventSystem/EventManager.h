#pragma once
#include "Event.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "WindowEvent.h"

#include <functional>
#include <vector>
#include <algorithm>

#define NAME(type) m_Subscriber##type
#define DECLARE(type) std::vector<std::function<bool(type##Event&)>> m_Subscriber##type
#define SUBSCRIBE(type)\
void Subscribe##type(std::function<bool(type##Event&)> func)\
{\
	NAME(type).push_back(func);\
}

namespace Firefly
{
	class EventManager
	{
	public:
		static EventManager* GetInstance() { return s_Instance; }
		static void Init();
		static void Terminate() { delete s_Instance; }

		void Handle(Event& e);

		SUBSCRIBE(KeyPress);
		SUBSCRIBE(KeyRelease);
		SUBSCRIBE(KeyRepeat);
		SUBSCRIBE(MouseScroll);
		SUBSCRIBE(WindowResize);
		SUBSCRIBE(WindowClose);
		
	private:
		static EventManager* s_Instance;
		DECLARE(KeyPress);
		DECLARE(KeyRelease);
		DECLARE(KeyRepeat);
		DECLARE(MouseScroll);
		DECLARE(WindowResize);
		DECLARE(WindowClose);
	private:
		EventManager() = default;
		~EventManager() = default;
	};
}

#undef NAME
#undef DECLARE
#undef FUNC
