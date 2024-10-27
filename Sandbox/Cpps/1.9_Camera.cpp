#include "Entrypoint.h"
#if CHECK_ACTIVE(1, 9, 0)

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/string_cast.hpp>
#include <iostream>

#include "0.0_Extentions/Updater/Updater.h"
#include "0.0_Extentions/IOSystem/EventSystem/EventManager.h"
#include "1.3_Hello_Window/Window.h"
#include "1.4_Hello_Triangle/IndexBuffer.h"
#include "1.4_Hello_Triangle/Shader.h"
#include "1.4_Hello_Triangle/VertexArray.h"
#include "1.4_Hello_Triangle/VertexBuffer.h"
#include "1.6_Texture/Texture.h"
#include "1.9_Camera/Projection.h"
#include "1.9_Camera/View.h"
using namespace Firefly;

int main()
{
	Window::Init(1200, 960);
	Window* windowInstance = Window::GetInstance();
	GLFWwindow* window = windowInstance->GetWindow();
	EventManager::Init();

	VertexArray vao;
	vao.Bind();

	float vertices[] = {
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
		 1.0f,  1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f, 0.0f,

		 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
		 1.0f,  1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		 1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		-1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 1.0f, 0.0f
	};
	VertexBuffer vbo(vertices, sizeof(vertices));
	vbo.Bind();

	unsigned int indices[] = {
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
		22, 23, 20
	};
	IndexBuffer ibo(indices, sizeof(indices));
	ibo.Bind();

	vao.SetLayout({
		{VertexArray::Type::Float, 3},
		{VertexArray::Type::Float, 2}
		});

	Shader shader({
		{Shader::Type::VertexShader, ASSET("1.9_Camera/camera_vertex.glsl")},
		{Shader::Type::FragmentShader, ASSET("1.9_Camera/camera_fragment.glsl")}
		});
	shader.Bind();

	Texture2D tex1(ASSET("1.9_Camera/awesomeface.png"));
	Texture2D tex2(ASSET("1.6_Texture/container.jpg"));

	tex1.Bind(0);
	tex2.Bind(1);

	glUniform1i(shader.GetLocation("tex1"), 1);

	glm::mat4 model = glm::mat4(1.0f);
	View view(glm::vec3(0.0f, 0.0f, 5.0f), true);
	view.SetSpeed({ 4.0f, 4.0f, 4.0f, 1.0f});
	EventManager::GetInstance()->SubscribeKeyPress(std::bind(&View::SetCursorMode, &view, std::placeholders::_1));

	std::array<float, 4> perParam = { static_cast<float>(glm::radians(45.0f)), 1200.0f / 960.0f, 0.1f, 100.0f };
	Projection* projection = new PerspectiveProjection(perParam);

	std::array<float, 6> orthoParam = { -5.0f, 5.0f, -4.0f, 4.0f, 0.0f, 10.0f };
	Projection* orthoprojection = new OrthoProjection(orthoParam);

	glEnable(GL_DEPTH_TEST);

	windowInstance->RenderLoop([&]() {
		Updater::Update();
		view.Update();
		glClearColor(0.2f, 0.6f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 pvm = projection->GetProjection() * view.GetView() * model;
		glUniformMatrix4fv(shader.GetLocation("transformation"), 1, GL_FALSE, &pvm[0][0]);

		glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);
		});
	Window::Terminate();
}
#endif