#pragma once
#include "Macros.h"
#include "Event.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "WindowEvent.h"

#include <functional>
#include <vector>
#include <algorithm>
#include <memory>
#include <mutex>

#define NAME(type) m_Subscriber##type
#define DECLARE(type) std::vector<std::function<bool(type##Event&)>> m_Subscriber##type
#define SUBSCRIBE(type)\
void Subscribe##type(std::function<bool(type##Event&)> func)\
{\
	NAME(type).push_back(func);\
}
#define UNSUBSCRIBE(type)\
void UnSubscribe##type(std::function<bool(type##Event&)> func)\
{\
    auto& subscribers = NAME(type);\
    for (auto it = subscribers.begin(); it != subscribers.end(); ++it)\
    {\
        if (*it == func)\
        {\
            subscribers.erase(it);\
            return;\
        }\
    }\
}

namespace Firefly
{
	class EventManager
	{
		SINGLETON(EventManager);
	public:
		void Handle(Event& e);

		SUBSCRIBE(KeyPress);
		SUBSCRIBE(KeyRelease);
		SUBSCRIBE(KeyRepeat);
		SUBSCRIBE(MouseScroll);
		SUBSCRIBE(WindowResize);
		SUBSCRIBE(WindowClose);
	private:
		DECLARE(KeyPress);
		DECLARE(KeyRelease);
		DECLARE(KeyRepeat);
		DECLARE(MouseScroll);
		DECLARE(WindowResize);
		DECLARE(WindowClose);
	private:
		EventManager();
		~EventManager() = default;
	};
}

#undef NAME
#undef DECLARE
#undef FUNC
