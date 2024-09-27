#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <memory>
#include <mutex>

#include "Macros.h"
#include "0.0_Extentions/IOSystem/EventSystem/Event.h"
#include "0.0_Extentions/IOSystem/EventSystem/WindowEvent.h"
#include "4.1_Depth_Test/RenderStatus.h"

namespace Firefly
{
	class Window
	{
		SINGLETON(Window);
	public:
		typedef std::function<void()> LoopFunc;
	public:
		void SetEventHandler(std::function<void(Event&)> handler) { m_EventHandler = handler; }
		inline GLFWwindow* GetWindow() { return m_Window; }
		void RenderLoop(LoopFunc func);
		void Render();

		// Polling
		GLenum GetKey(GLenum key) const { return glfwGetKey(m_Window, key); }
		GLenum GetMouseButton(GLenum button) const { return glfwGetMouseButton(m_Window, button); }
		void GetCursorPos(double* xpos, double* ypos) const { glfwGetCursorPos(m_Window, xpos, ypos); }

		bool OnWindowResizeEvent(WindowResizeEvent& e);

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		void SetRatio(float ratio) { m_Ratio = ratio; }
	private:
		GLFWwindow* m_Window;
		int m_Width, m_Height;
		float m_Ratio;
		std::function<void(Event&)> m_EventHandler;
	private:
		Window(unsigned int width = 800, unsigned int height = 800);
		~Window();

		void Construct();
		void RegistCallback();
	};
}