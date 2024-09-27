#include "EventManager.h"
#include "1.3_Hello_Window/Window.h"

namespace Firefly
{
	EventManager* EventManager::s_Instance = nullptr;
	
#define DISPATCH(type)\
for (auto it = m_Subscriber##type.begin(); it != m_Subscriber##type.end() && !e.m_IsHandled; it++)\
{\
	(*it)(*dynamic_cast<type##Event*>(&e));\
}\
break;	

	void EventManager::Init()
	{
		s_Instance = new EventManager();
		Window::GetInstance().SetEventHandler(std::bind(&EventManager::Handle, s_Instance, std::placeholders::_1));
		s_Instance->SubscribeWindowResize(std::bind(&Window::OnWindowResizeEvent, &Window::GetInstance(), std::placeholders::_1));
	}

	void EventManager::Handle(Event& e)
	{
		switch (e.GetType())
		{
		case EventType::KeyPress:		DISPATCH(KeyPress);
		case EventType::KeyRelease:		DISPATCH(KeyRelease);
		case EventType::KeyRepeat:		DISPATCH(KeyRepeat);
		case EventType::MouseScroll:	DISPATCH(MouseScroll);
		case EventType::WindowResize:	DISPATCH(WindowResize);
		}
	}
#undef DISPATCH
}