#include "View.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "1.3_Hello_Window/Window.h"
#include "0.0_Extentions/Updater/MousePos.h"
#include "0.0_Extentions/IOSystem/EventSystem/EventManager.h"
#include "0.0_Extentions/Updater/Time.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/string_cast.hpp>
#include <gtc/matrix_transform.hpp>

namespace Firefly
{
	View::View(const glm::vec3& position, const glm::vec3& direction, bool enableRotate, float pitchMax)
		: m_Position(position), m_Direction(direction), m_EnableRotate(enableRotate), PITCH_MAX(pitchMax),
		m_Pitch(0.0f), m_Yaw(0.0f), m_SpeedX(0.0f), m_SpeedY(0.0f), m_SpeedZ(0.0f), m_RotateSensitive(0.0f)
	{
		m_Pitch = asin(direction.y / glm::length(direction));
		m_Yaw = asin(-direction.x / glm::length(glm::vec2(direction.x, direction.z)));
		Recalculate();
		EventManager::GetInstance().SubscribeKeyPress(std::bind(&View::SetCursorMode, this, std::placeholders::_1));
	}

	void View::Update()
	{
		glm::vec3 front = glm::vec3(-sin(m_Yaw), 0.0f, -cos(m_Yaw));
		glm::vec3 right = glm::vec3(cos(m_Yaw), 0.0f, -sin(m_Yaw));

		float deltaTime = Time::GetInstance().GetDeltaTime();

		if (Window::GetInstance().GetKey(GLFW_KEY_W))
		{
			m_Position += m_SpeedX * deltaTime * front;
		}
		else if(Window::GetInstance().GetKey(GLFW_KEY_S))
		{
			m_Position -= m_SpeedX * deltaTime * front;
		}
		if (Window::GetInstance().GetKey(GLFW_KEY_D))
		{
			m_Position += m_SpeedX * deltaTime * right;
		}
		else if (Window::GetInstance().GetKey(GLFW_KEY_A))
		{
			m_Position -= m_SpeedX * deltaTime * right;
		}
		if (Window::GetInstance().GetKey(GLFW_KEY_SPACE))
		{
			m_Position.y += m_SpeedZ * deltaTime;
		}
		else if (Window::GetInstance().GetKey(GLFW_KEY_LEFT_SHIFT))
		{
			m_Position.y -= m_SpeedZ * deltaTime;
		}

		if (m_EnableRotate)
		{
			float deltaX = MousePos::GetInstance().GetDeltaX();
			float deltaY = MousePos::GetInstance().GetDeltaY();

			m_Pitch -= deltaY * deltaTime * m_RotateSensitive;
			m_Yaw -= deltaX * deltaTime * m_RotateSensitive;
			m_Pitch =
				m_Pitch > PITCH_MAX ? PITCH_MAX :
				m_Pitch < -PITCH_MAX ? -PITCH_MAX :
				m_Pitch;
		}
		Recalculate();
	}
	
	bool View::SetCursorMode(KeyPressEvent& e)
	{
		if (e.GetKey() == GLFW_KEY_ESCAPE)
		{
			m_EnableRotate = !m_EnableRotate;
		}
		return false;
	}
	
	void View::Recalculate()
	{
		m_Direction = glm::vec3(-cos(m_Pitch) * sin(m_Yaw), sin(m_Pitch), -cos(m_Pitch) * cos(m_Yaw));
		glm::vec3 up = glm::vec3(sin(m_Pitch) * sin(m_Yaw), cos(m_Pitch), sin(m_Pitch) * cos(m_Yaw));
		m_View = glm::lookAt(m_Position, m_Position + m_Direction, up);
	}
}