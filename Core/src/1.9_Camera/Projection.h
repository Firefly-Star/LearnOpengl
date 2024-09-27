#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <array>
#include <variant>

namespace Firefly
{
	using Varray = std::variant<std::array<float, 4>, std::array<float, 6>>;
	class Projection
	{
	public:
		Projection() : m_Projection() {}
		virtual ~Projection() {}
		const glm::mat4& GetProjection() { return m_Projection; }
		virtual void SetParams(const Varray& params) = 0;
	protected:
		glm::mat4 m_Projection;
		virtual void Recalculate() = 0;
	};

	class OrthoProjection : public Projection
	{
	public:
		OrthoProjection() : m_Params({ -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f }) { Recalculate(); }
		OrthoProjection(const std::array<float, 6>& params) : m_Params(params) { Recalculate(); }
		virtual void SetParams(const Varray& params) override
		{
			if (auto p = std::get_if<std::array<float, 6>>(&params))
			{
				m_Params = *p;
				Recalculate();
			}
		}
	protected:
		virtual void Recalculate() override
		{
			m_Projection = glm::ortho(m_Params[0], m_Params[1], m_Params[2], m_Params[3], m_Params[4], m_Params[5]);
		}
	private:
		std::array<float, 6> m_Params;
	};

	class PerspectiveProjection : public Projection
	{
	public:
		PerspectiveProjection() : m_Params({ static_cast<float>(glm::radians(45.0f)), 1.0f, 0.1f, 100.0f }) { Recalculate(); }
		PerspectiveProjection(const std::array<float, 4>& params) : m_Params(params) { Recalculate(); }
		virtual void SetParams(const Varray& params) override
		{
			if (auto p = std::get_if<std::array<float, 4>>(&params))
			{
				m_Params = *p;
				Recalculate();
			}
		}
	protected:
		virtual void Recalculate() override
		{
			m_Projection = glm::perspective(m_Params[0], m_Params[1], m_Params[2], m_Params[3]);
		}
	private:
		std::array<float, 4> m_Params;
	};

}