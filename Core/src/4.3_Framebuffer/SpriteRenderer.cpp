#include "SpriteRenderer.h"
#include <glad/glad.h>

namespace Firefly
{
	std::unique_ptr<SpriteRenderer> SpriteRenderer::s_Instance = nullptr;
	std::once_flag SpriteRenderer::s_OnceFlag;

	static float vertices[] =
	{
		-1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 1.0f, 0.0f
	};

	static unsigned int indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	void SpriteRenderer::Render()
	{
		m_VAO.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

	SpriteRenderer::SpriteRenderer()
		: m_VAO(), m_VBO(vertices, sizeof(vertices)), m_IBO(indices, sizeof(indices))
	{
		m_VAO.SetLayout({
			{
				{
					{ VertexArray::Type::Float, 2 },
					{ VertexArray::Type::Float, 2 }
				}, m_VBO
			}
			});
		m_VAO.UnBind();
	}
	
	SpriteRenderer::~SpriteRenderer()
	{
	}
}