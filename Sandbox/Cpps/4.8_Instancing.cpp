#include "EntryPoint.h"
#if CHECK_ACTIVE(4, 8, 0)

#include "Core.h"
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include "glm.hpp"
using namespace Firefly;
int main()
{
	Window::Init(1000, 1000);
	Window& windowInstance = Window::GetInstance();
	BindManager::Init();
	EventManager::Init();

	Model planet(ASSET("4.8_Instancing/planet/planet.obj"));
	Model rock(ASSET("4.8_Instancing/rock/rock.obj"));
	Shader modelShader({
		{Shader::Type::VertexShader, ASSET("4.8_Instancing/model_vertex.glsl")},
		{Shader::Type::FragmentShader, ASSET("4.8_Instancing/model_fragment.glsl")}
		});
	Shader instanceShader({
		{Shader::Type::VertexShader, ASSET("4.8_Instancing/instance_vertex.glsl")},
		{Shader::Type::FragmentShader, ASSET("4.8_Instancing/model_fragment.glsl")}
		});
	UniformBuffer ubo(sizeof(glm::mat4));
	modelShader.BindUniformBlock("Mat", 4);
	instanceShader.BindUniformBlock("Mat", 4);
	ubo.Bind(4);
	
	auto proj = Projection::Create<Projection::Type::Perspective>(std::array<float, 4>({ glm::radians(45.0f), 1.0f, 0.1f, 100.0f }));
	View view(glm::vec3(0.0f, 0.0f, 20.0f));
	view.SetSpeed({ 5.0f, 5.0f, 5.0f, 0.3f });

	glm::mat4 modelMatPlanet(1.0f);

	float radius = 10.0f;
	int count = 100000;
	glm::mat4* modelMatRock = new glm::mat4[count];
	srand(1);
	glm::mat4 e(1.0f);
	std::cout <<
		(float*)(&e[0]) - (float*)(&e[0][0]) << " " <<
		(float*)(&e[1]) - (float*)(&e[0]) << " " <<
		(float*)(&e[2]) - (float*)(&e[1]) << " " <<
		(float*)(&e[3]) - (float*)(&e[2]) << "\n";
	auto getRandom = [](float lbound = 0.0f, float ubound = 1.0f) {
		return static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (ubound - lbound) + lbound;
		};
	for (int i = 0; i < count; ++i)
	{
		float manitude = getRandom(0.02f, 0.1f);
		float radiusi = radius + getRandom(-4.0f, 2.0f);
		glm::vec3 translation = glm::vec3(
			radiusi * sin(glm::radians(360.0f / count * i)), 
			getRandom(-1.0f, 1.0f), 
			radiusi * cos(glm::radians(360.0f / count * i)));
		modelMatRock[i] = 
			glm::translate(e, translation) 
			* glm::rotate(e, glm::radians(rand() + 0.0f), glm::vec3(rand(), rand(), rand())) 
			* glm::scale(e, manitude * glm::vec3(1.0f, 1.0f, 1.0f));
	}
	VertexBuffer instanceVbo(modelMatRock, sizeof(glm::mat4) * count);
	rock.AppendLayout({
		{
			{
				{VertexArray::Type::Float, 4},
				{VertexArray::Type::Float, 4},
				{VertexArray::Type::Float, 4},
				{VertexArray::Type::Float, 4}
			}, instanceVbo, 1
		}
		});
	
	glEnable(GL_DEPTH_TEST);

	windowInstance.RenderLoop([&]() {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Updater::Update();
		view.Update();

		glm::mat4 pvMat = proj->GetProjection() * view.GetView();
		ubo.FillSub(0, sizeof(glm::mat4), &pvMat[0][0]);

		// planet
		modelShader.Bind();
		modelShader.SetUniform("modelMat", modelMatPlanet);
		planet.Render(modelShader);

		// rock
		rock.RenderInstance(instanceShader, count);

		// print frame rate
		std::cout << "frame rate: " << 1.0f / Time::GetDeltaTime() << "\n";
		});

	EventManager::Terminate();
	Window::Terminate();
}

#endif