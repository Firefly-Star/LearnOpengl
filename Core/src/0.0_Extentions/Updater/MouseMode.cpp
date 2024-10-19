#include "MouseMode.h"
#include "0.0_Extentions/IOSystem/EventSystem/KeyEvent.h"
#include "0.0_Extentions/IOSystem/EventSystem/EventManager.h"
#include "1.3_Hello_Window/Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Firefly
{
	SINGLETON_DEFINATION(MouseMode);
	
	void MouseMode::SetKey(int key)
	{
		m_Key = key;
	}

	MouseMode::MouseMode(int key)
		:m_Key(key <= 0 ? GLFW_KEY_ESCAPE : key), m_Hide(true)
	{
		KeyPressEvent e(-1, -1);
		OnEvent(e);
		EventManager::GetInstance().SubscribeKeyPress(std::bind(&MouseMode::OnEvent, this, std::placeholders::_1));
	}
	
	MouseMode::~MouseMode()
	{
	}
	
	bool MouseMode::OnEvent(KeyPressEvent& e)
	{
		if (e.GetKey() == m_Key)
		{
			m_Hide = !m_Hide;
		}
		if (m_Hide)
		{
			glfwSetInputMode(Window::GetInstance().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
		{
			glfwSetInputMode(Window::GetInstance().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		return false;
	}
}