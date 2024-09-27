#include "Window.h"
#include "0.0_Extentions/IOSystem/EventSystem/KeyEvent.h"
#include "0.0_Extentions/IOSystem/EventSystem/MouseEvent.h"
#include "0.0_Extentions/IOSystem/EventSystem/WindowEvent.h"
#include <iostream>

namespace Firefly
{
	std::unique_ptr<Window> Window::s_Instance = nullptr;
	std::once_flag Window::s_OnceFlag;

	Window::Window(unsigned int width, unsigned int height)
		:m_Width(width), m_Height(height), m_ClearFlag(GL_COLOR_BUFFER_BIT), m_Ratio((width + 0.0f) / height)
	{
		Construct();
		RegistCallback();
	}

	void Window::RenderLoop(LoopFunc func)
	{
		while (!glfwWindowShouldClose(m_Window))
		{
			func();
			Render();
		}
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	void Window::Construct()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow(m_Width, m_Height, "Learning opengl", nullptr, nullptr);
		if (m_Window == nullptr)
		{
			std::cout << "Failed to create glfw window! \n";
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(m_Window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize glad! \n";
		}
		const GLubyte* version = glGetString(GL_VERSION);
		std::cout << "OpenGL version: " << version << std::endl;

		glfwSetWindowUserPointer(m_Window, &m_EventHandler);
	}

	void Window::RegistCallback()
	{
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			auto& handler = *reinterpret_cast<std::function<void(Event&)>*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS :
			{
				KeyPressEvent e(key, mods);
				handler(e);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyRepeatEvent e(key);
				handler(e);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleaseEvent e(key);
				handler(e);
				break;
			}
			}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
			auto& handler = *reinterpret_cast<std::function<bool(Event&)>*>(glfwGetWindowUserPointer(window));
			MouseScrollEvent e(yoffset);
			handler(e);
			});

		glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			auto& handler = *reinterpret_cast<std::function<bool(Event&)>*>(glfwGetWindowUserPointer(window));
			WindowResizeEvent e(width, height);
			handler(e);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			auto& handler = *reinterpret_cast<std::function<bool(Event&)>*>(glfwGetWindowUserPointer(window));
			WindowCloseEvent e;
			handler(e);
			});
	}

	void Window::Render()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
		glClear(m_ClearFlag);
	}

	bool Window::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		m_Width = e.GetWidth();
		m_Height = e.GetHeight();
		if (m_Width != 0 && m_Height != 0)
		{
			if (m_Width / m_Height >= m_Ratio)
			{
				glViewport(static_cast<int>((m_Width - m_Height * m_Ratio) / 2), 0, static_cast<int>(m_Height * m_Ratio), m_Height);
			}
			else
			{
				glViewport(0, static_cast<int>((m_Height - m_Width / m_Ratio) / 2), m_Width, static_cast<int>(m_Width / m_Ratio));
			}
		}
		return false;
	}

	void Window::Enable(RenderStatus::Status status)
	{
		if (status & RenderStatus::DepthTest)
		{
			glEnable(GL_DEPTH_TEST);
			m_ClearFlag |= GL_DEPTH_BUFFER_BIT;
		}
		if (status & RenderStatus::StencilTest)
		{
			glEnable(GL_STENCIL_TEST);
			m_ClearFlag |= GL_STENCIL_BUFFER_BIT;
		}
	}
	
	void Window::Disable(RenderStatus::Status status)
	{
		if (status & RenderStatus::DepthTest)
		{
			glDisable(GL_DEPTH_TEST);
			m_ClearFlag &= ~GL_DEPTH_BUFFER_BIT;
		}
		if (status & RenderStatus::StencilTest)
		{
			glDisable(GL_STENCIL_TEST);
			m_ClearFlag &= ~GL_STENCIL_BUFFER_BIT;
		}
	}
}