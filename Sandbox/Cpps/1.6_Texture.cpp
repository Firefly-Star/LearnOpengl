#include "Entrypoint.h"
#if CHECK_ACTIVE(1, 6, 0)

#include "1.3_Hello_Window/Window.h"
#include "1.4_Hello_Triangle/IndexBuffer.h"
#include "1.4_Hello_Triangle/Shader.h"
#include "1.4_Hello_Triangle/VertexArray.h"
#include "1.4_Hello_Triangle/VertexBuffer.h"
#include "1.6_Texture/Texture.h"
using namespace Firefly;

int main()
{
	Window windowInstance(1200, 960);
	GLFWwindow* window = windowInstance.GetWindow();
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
		});
	
	VertexArray vao;
	
	float vertices[] = {
		/*-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f*/
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f
	};
	VertexBuffer vbo(vertices, sizeof(vertices));
	vbo.Bind();

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	IndexBuffer ibo(indices, sizeof(indices));
	ibo.Bind();

	vao.SetLayout({ { VertexArray::Type::Float, 3 }, { VertexArray::Type::Float, 3 }, { VertexArray::Type::Float, 2 } });

	std::vector<Shader::File> files =
	{
		{Shader::Type::VertexShader, "C:/Users/Summer/Desktop/OpenGL/OpenGL/src/assets/1.6_Texture/texture_vertex.glsl"},
		{Shader::Type::FragmentShader, "C:/Users/Summer/Desktop/OpenGL/OpenGL/src/assets/1.6_Texture/texture_fragment.glsl"}
	};
	Shader shader(files);
	shader.Bind();

	Texture2D textureWall("C:/Users/Summer/Desktop/OpenGL/OpenGL/src/assets/1.6_Texture/wall.jpg");
	Texture2D textureContainer("C:/Users/Summer/Desktop/OpenGL/OpenGL/src/assets/1.6_Texture/container.jpg");

	textureWall.Bind(1);
	textureContainer.Bind(2);
	glUniform1i(shader.GetLocation("tex1"), 1);
	glUniform1i(shader.GetLocation("tex2"), 2);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.6f, 0.4f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, 0);

		windowInstance.Render();
	}
}

#endif