#include "Entrypoint.h"
#if ACTIVE_LESSON_MAJOR == 1 && ACTIVE_LESSON_MINOR == 7 && TEST == 0

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>

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

	VertexArray vao;
	vao.Bind();

	float vertices[] = {
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

	vao.SetLayout({
		{VertexArray::Type::Float, 3},
		{VertexArray::Type::Float, 3},
		{VertexArray::Type::Float, 2}
		});

	Shader shader({
		{Shader::Type::VertexShader, ASSET("1.7_Transformation/transformation_vertex.glsl")},
		{Shader::Type::FragmentShader, ASSET("1.7_Transformation/transformation_fragment.glsl")}
		});
	shader.Bind();

	Texture2D tex1(ASSET("1.6_Texture/wall.jpg"));

	tex1.Bind(0);

	glUniform1i(shader.GetLocation("tex1"), 0);

	glm::mat4 model = glm::mat4(1.0f);
	double currentTime = 0.0;

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.6f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		model = glm::rotate(model, static_cast<float>(glm::radians((glfwGetTime() - currentTime) * 90)), glm::vec3({ 0.0f, 0.0f, 1.0f }));
		currentTime = glfwGetTime();
		glUniformMatrix4fv(shader.GetLocation("transformation"), 1, GL_FALSE, &model[0][0]);

		glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);

		windowInstance.Render();
	}
}

#endif