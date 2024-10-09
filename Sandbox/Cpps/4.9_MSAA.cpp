#include "Entrypoint.h"
#if CHECK_ACTIVE(4, 9, 0)

#include "Core.h"
#include <glad/glad.h>

using namespace Firefly;
int main()
{
	Window::Init(1000, 1000);
	BindManager::Init();
	EventManager::Init();
	ShaderLibrary::Init();

	Window& windowInstance = Window::GetInstance();

	float vertices[] = {
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
		0, 3, 7,
		7, 4, 0,
		1, 2, 6,
		6, 5, 1,
		0, 1, 5,
		5, 4, 0,
		3, 2, 6,
		6, 7, 3
	};

	VertexArray vao;
	VertexBuffer vbo(vertices, sizeof(vertices));
	IndexBuffer ibo(indices, sizeof(indices));
	vao.SetLayout({
		{
			{
				{VertexArray::Type::Float, 3}
			}, vbo
		}
		});

	Shader basicShader({
		{Shader::Type::VertexShader, ASSET("4.9_MSAA/basic_vertex.glsl")},
		{Shader::Type::FragmentShader, ASSET("4.9_MSAA/basic_fragment.glsl")}
		});

	auto proj = Projection::Create<Projection::Type::Perspective>(std::array<float, 4>({ glm::radians(45.0f), 1.0f, 0.1f, 100.0f }));
	View view({ 0.0f, 0.0f, 5.0f });
	view.SetSpeed({ 1.0f, 1.0f, 1.0f, 0.2f });

	MultiSampleFramebuffer multifbo(1000, 1000, 4);

	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_MULTISAMPLE);

	Updater::Update();
	int count = 0;
	float time = Time::GetTime();
	windowInstance.RenderLoop([&]() {

		multifbo.Bind();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Updater::Update();
		view.Update();
		basicShader.Bind();
		basicShader.SetUniform("pvMat", proj->GetProjection() * view.GetView());
		basicShader.SetUniform("modelMat", glm::mat4(1.0f));
		vao.Bind();
		glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);
		++count;
		if (count == 1000)
		{
			float temp = Time::GetTime();
			std::cout << "Average frame rate: " << 1000 / (temp - time) << "\n";
			time = temp;
			count = 0;
		}
		
		multifbo.UnBind();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		multifbo.CopyTo(0);
		});
	
	ShaderLibrary::Terminate();
	EventManager::Terminate();
	BindManager::Terminate();
	Window::Terminate();
}

#endif