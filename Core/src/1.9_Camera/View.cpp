#include "View.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "0.0_Extentions/Updater/MousePos.h"
#include "0.0_Extentions/Updater/Time.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/string_cast.hpp>
#include <gtc/matrix_transform.hpp>

namespace Firefly
{
	View::View(const glm::vec3& position, bool enableRotate, float pitch, float yaw, float pitchMax)
		:m_Pitch(pitch), m_Yaw(yaw), m_Position(position), PITCH_MAX(pitchMax), 
		m_SpeedX(0.0f), m_SpeedY(0.0f), m_RotateSensitive(0.0f), m_EnableRotate(enableRotate)
	{
		Recalculate();
		KeyPressEvent e(-1, -1);
		SetCursorMode(e);
	}
	void View::Update()
	{
		float deltaTime = Time::GetDeltaTime();

		glm::vec3 moveDirection(-sin(m_Yaw), 0.0f, -cos(m_Yaw));
		glm::vec3 moveRight(cos(m_Yaw), 0.0f, -sin(m_Yaw));
		if (Window::GetInstance().GetKey(GLFW_KEY_W) == GLFW_PRESS)
		{
			m_Position += moveDirection * m_SpeedZ * deltaTime;
		}
		else if (Window::GetInstance().GetKey(GLFW_KEY_S) == GLFW_PRESS)
		{
			m_Position -= moveDirection * m_SpeedZ * deltaTime;
		}
		if (Window::GetInstance().GetKey(GLFW_KEY_D) == GLFW_PRESS)
		{
			m_Position += moveRight * m_SpeedX * deltaTime;
		}
		else if (Window::GetInstance().GetKey(GLFW_KEY_A) == GLFW_PRESS)
		{
			m_Position -= moveRight * m_SpeedX * deltaTime;
		}
		if (Window::GetInstance().GetKey(GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			m_Position.y += m_SpeedY * deltaTime;
		}
		else if (Window::GetInstance().GetKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			m_Position.y -= m_SpeedY * deltaTime;
		}

		if (m_EnableRotate)
		{
			float deltaX = MousePos::GetDeltaX();
			float deltaY = MousePos::GetDeltaY();

			

			m_Yaw -= deltaX * deltaTime * m_RotateSensitive;
			m_Pitch -= deltaY * deltaTime * m_RotateSensitive;
		}

		Recalculate();
	}

	bool View::SetCursorMode(KeyPressEvent& e)
	{
		if (e.GetKey() == GLFW_KEY_ESCAPE)
		{
			m_EnableRotate = !m_EnableRotate;
		}
		if (m_EnableRotate)
		{
			glfwSetInputMode(Window::GetInstance().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
		{
			glfwSetInputMode(Window::GetInstance().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		return false;
	}

	void View::Recalculate()
	{
		glm::vec3 right(cos(m_Yaw), 0.0f, -sin(m_Yaw));
		m_Direction = glm::vec3(-sin(m_Yaw) * cos(m_Pitch), sin(m_Pitch), -cos(m_Pitch) * cos(m_Yaw));
		m_View = glm::lookAt(m_Position, m_Position + m_Direction, glm::cross(right, m_Direction));
	}
}