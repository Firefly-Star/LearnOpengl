#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <array>
#include "0.0_Extentions/IOSystem/EventSystem/KeyEvent.h"


namespace Firefly
{
	// Pitch: (1.0f, 0.0f, 0.0f), Yaw: (0.0f, 1.0f, 0.0f), look at (0.0f, 0.0f, -1.0f)
	class View
	{
	public:
		View(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), bool enableRotate = true, float pitch = 0.0f, float yaw = 0.0f, float pitchMax = 89.9f);
		void SetSpeed(const std::array<float, 4>& speed = {1.0f, 1.0f, 1.0f, 1.0f})
		{
			m_SpeedX = speed[0]; m_SpeedY = speed[1]; m_SpeedZ = speed[2]; m_RotateSensitive = speed[3];
		}
		void Update();
		const glm::mat4& GetView() { return m_View; }
		const glm::vec3& GetPosition() { return m_Position; }
		const glm::vec3& GetDirection() { return m_Direction; }
		bool SetCursorMode(KeyPressEvent& e);
	private:
		glm::vec3 m_Position, m_Direction;
		glm::mat4 m_View;
		float m_Pitch, m_Yaw;
		const float PITCH_MAX;
		bool m_EnableRotate;

		float m_SpeedX, m_SpeedY, m_SpeedZ, m_RotateSensitive;
	private:
		void Recalculate();
	};
}