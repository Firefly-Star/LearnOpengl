#include "Entrypoint.h"
#if CHECK_ACTIVE(2, 2, 1)

#include "Core.h"
#include <glad/glad.h>

using namespace Firefly;
int main()
{
	Window::Init();
	Window& windowInstance = Window::GetInstance();
	SingletonManager::Init();

	float vertices[] = {
		-0.5f, -0.5f,
		-0.5f,  0.5f,
		 0.5f,  0.5f,
		 0.5f, -0.5f
	};

	unsigned int indices1[] = {
		0, 1, 2,
		2, 3, 0
	};

	unsigned int indices2[] = {
		0, 1, 2
	};

	VertexArray vao;
	vao.Bind();
	VertexBuffer vbo(vertices, sizeof(vertices));
	IndexBuffer ibo(indices1, sizeof(indices1));
	vao.SetLayout({
		{
			{
				{VertexArray::Type::Float, 2}
			}, vbo
		}
		});
	ibo.Bind();
	vao.UnBind();
	IndexBuffer ibo2(indices2, sizeof(indices2));
	ibo2.Bind();

	Shader shader({
		{Shader::Type::VertexShader, ASSET("1.5_Shader/uniform_vertex.glsl")},
		{Shader::Type::FragmentShader, ASSET("1.5_Shader/uniform_fragment.glsl")}
		});

	windowInstance.RenderLoop([&]() {
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Bind();
		vao.Bind();
		glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);
		});
	SingletonManager::Terminate();
}

#endif