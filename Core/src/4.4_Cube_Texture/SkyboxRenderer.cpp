#include "SkyboxRenderer.h"
#include <glad/glad.h>
namespace Firefly
{
	std::unique_ptr<SkyboxRenderer> SkyboxRenderer::s_Instance = nullptr;
	std::once_flag SkyboxRenderer::s_OnceFlag;

	static float vertices[] = {
		// Right
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		// Left
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		// Top
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		// Bottom
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		// Bebind
		-1.0f, -1.0f,  -1.0f,
		-1.0f,  1.0f,  -1.0f,
		 1.0f,  1.0f,  -1.0f,
		 1.0f, -1.0f,  -1.0f,
		// Front
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
	};
	static unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20,
	};

	void SkyboxRenderer::Render()
	{
		m_VAO.Bind();
		glDrawElements(GL_TRIANGLES, m_IBO.GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	SkyboxRenderer::SkyboxRenderer()
		:m_VAO(), m_VBO(vertices, sizeof(vertices)), m_IBO(indices, sizeof(indices))
	{
		m_VAO.SetLayout({
			{
				{
					{VertexArray::Type::Float, 3}
				}, m_VBO
			}
			});
	}

	SkyboxRenderer::~SkyboxRenderer()
	{
	}

}