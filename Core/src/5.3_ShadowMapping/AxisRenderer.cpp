#include "AxisRenderer.h"

#include "1.3_Hello_Window/Window.h"
#include "1.9_Camera/Projection.h"
#include "4.3_Framebuffer/ShaderLibrary.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

namespace Firefly
{
	SINGLETON_DEFINATION(AxisRenderer);

	static float vertices[] = {
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};
	static unsigned int indices[] = {
		0, 1,
		2, 3,
		4, 5
	};

	AxisRenderer::AxisRenderer()
		:m_VAO(), m_VBO(vertices, sizeof(vertices)), m_IBO(indices, sizeof(indices))
	{
		m_VAO.SetLayout({
			{
				{
					{VertexArray::Type::Float, 3},
					{VertexArray::Type::Float, 3}
				}, m_VBO
			}
			});
		m_VAO.UnBind();
	}

	AxisRenderer::~AxisRenderer()
	{
	}

	void AxisRenderer::Render(const View& view, float x, float y, float size)
	{
		glDisable(GL_DEPTH_TEST);
		glm::mat4 e(1.0f);

		float width = Window::GetInstance().GetWidth();
		float height = Window::GetInstance().GetHeight();

		glm::mat4 modelMat =
			glm::translate(e, glm::vec3(x, y, 0.0f)) * 
			glm::scale(e, glm::vec3(size, size, size));

		glm::mat4 viewMat = glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f) + view.GetDirection(), view.GetUp());

		OrthoProjection projection(std::array<float, 6>{0.0f, width, 0.0f, height, -size * 2, size * 2});

		glm::mat4 pvmMat = projection.GetProjection() * viewMat * modelMat;

		Shader& axisShader = ShaderLibrary::GetInstance().Get("Axis1D");
		axisShader.Bind();
		axisShader.SetUniform("pvmMat", pvmMat);
		m_VAO.Bind();
		glDrawElements(GL_LINES, m_IBO.GetCount(), GL_UNSIGNED_INT, nullptr);
		glEnable(GL_DEPTH_TEST);
	}

}