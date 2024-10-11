#include "Time.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Firefly
{
	std::unique_ptr<Time> Time::s_Instance = nullptr;
	std::once_flag Time::s_OnceFlag;

	Time::Time()
		:m_Time(0.0f), m_Deltatime(1.0f / 60.0f)
	{
	}

	void Time::Update()
	{
		float nowTime = static_cast<float>(glfwGetTime());
		m_Deltatime = nowTime - m_Time;
		m_Time = nowTime;
	}
}