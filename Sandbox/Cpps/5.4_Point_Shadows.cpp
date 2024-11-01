#include "Entrypoint.h"
#if CHECK_ACTIVE(5, 4, 0)

#include "Core.h"

#include <iostream>
#include <glad/glad.h>
#include "crtdbg.h"
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

using namespace Firefly;

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Window::Init(1200, 1200);
	SingletonManager::Init();

	Shader depthShader({
		{Shader::Type::VertexShader, ASSET("5.4_Point_Shadow/point_depth_vertex.glsl")},
		{Shader::Type::GeometryShader, ASSET("5.4_Point_Shadow/point_depth_geometry.glsl")},
		{Shader::Type::FragmentShader, ASSET("5.4_Point_Shadow/point_depth_fragment.glsl")}
		});
	Shader displayShader({
		{Shader::Type::VertexShader, ASSET("5.4_Point_Shadow/display_vertex.glsl")},
		{Shader::Type::FragmentShader, ASSET("5.4_Point_Shadow/display_fragment.glsl")}
		});
	Shader basicShader({
		{Shader::Type::VertexShader, ASSET("5.4_Point_Shadow/basic_vertex.glsl")},
		{Shader::Type::FragmentShader, ASSET("5.4_Point_Shadow/basic_fragment.glsl")}
		});
	Shader shadowShader({
		{Shader::Type::VertexShader, ASSET("5.4_Point_Shadow/shadow_vertex.glsl")},
		{Shader::Type::FragmentShader, ASSET("5.4_Point_Shadow/shadow_fragment.glsl")}
		});
	Shader testShader({
		{Shader::Type::VertexShader, ASSET("5.4_Point_Shadow/shadow_vertex.glsl")},
		{Shader::Type::FragmentShader, ASSET("5.4_Point_Shadow/test_fragment.glsl")}
		});
	Shader test2Shader({
		{Shader::Type::VertexShader, ASSET("5.4_Point_Shadow/shadow_vertex.glsl")},
		{Shader::Type::FragmentShader, ASSET("5.4_Point_Shadow/test_fragment2.glsl")}
		});

	unsigned int fboSize = 500;
	DepthCubeFramebuffer cubeDepthFBO(fboSize);

	Model ourModel(ASSET("3.2_Model/nanosuit/nanosuit.obj"));
	PerspectiveProjection lightPerspective({ glm::radians(90.0f), 1.0f, 0.1f, 100.0f });

	glm::mat4 lightpvMat[6] = {
		lightPerspective.GetProjection() * glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		lightPerspective.GetProjection() * glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		lightPerspective.GetProjection() * glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
		lightPerspective.GetProjection() * glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
		lightPerspective.GetProjection() * glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		lightPerspective.GetProjection() * glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
	};

	const glm::mat4 e(1.0f);
	glm::mat4 modelModelMat[6] = {
		glm::translate(e, glm::vec3( 10.0f, 0.0f, 0.0f)) * glm::scale(e, glm::vec3(0.5f, 0.5f, 0.5f)),
		glm::translate(e, glm::vec3(-10.0f, 0.0f, 0.0f)) * glm::scale(e, glm::vec3(0.5f, 0.5f, 0.5f)),
		glm::translate(e, glm::vec3(0.0f,  10.0f, 0.0f)) * glm::scale(e, glm::vec3(0.5f, 0.5f, 0.5f)),
		glm::translate(e, glm::vec3(0.0f, -10.0f, 0.0f)) * glm::scale(e, glm::vec3(0.5f, 0.5f, 0.5f)),
		glm::translate(e, glm::vec3(0.0f, 0.0f,  10.0f)) * glm::scale(e, glm::vec3(0.5f, 0.5f, 0.5f)),
		glm::translate(e, glm::vec3(0.0f, 0.0f, -10.0f)) * glm::scale(e, glm::vec3(0.5f, 0.5f, 0.5f))
	};

	glm::mat4 cubeModelMat;

	float cubeVertices[] = {
		-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

		-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		 1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		 1.0f,  1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f,  1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,

		 1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  -1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  -1.0f, 1.0f, 0.0f,
	};
	unsigned int cubeIndices[] = {
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

	VertexArray cubeVAO;
	VertexBuffer cubeVBO(cubeVertices, sizeof(cubeVertices));
	IndexBuffer cubeIBO(cubeIndices, sizeof(cubeIndices));
	cubeVAO.Bind();
	cubeVAO.SetLayout({
		{
			{
				{VertexArray::Type::Float, 3},
				{VertexArray::Type::Float, 3},
				{VertexArray::Type::Float, 2},
			}, cubeVBO
		}
		});
	cubeIBO.Bind();
	cubeVAO.UnBind();

	Texture2D wood(ASSET("5.3_ShadowMapping/wood.png"));

	unsigned char whiteTexture[] = {
		255, 255, 255, 255
	};
	Texture2D pureWhite(whiteTexture, 1, 1);

	PerspectiveProjection ourProjection({ glm::radians(45.0f), 1.0f, 0.1f, 100.0f });
	View ourView;
	ourView.SetSpeed({ 10.0f, 10.0f, 10.0f, 1.0f });
	Window::GetInstance().RenderLoop([&]() {
		Updater::Update();
		ourView.Update();

		// Depth Mapping
		cubeDepthFBO.Bind();
		glViewport(0, 0, fboSize, fboSize);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		depthShader.Bind();
		for (int i = 0; i < 6; ++i)
		{
			depthShader.SetUniform(("pvMat[" + to_string(i) + "]").c_str(), lightpvMat[i]);
		}
		depthShader.SetUniform("lightPos", glm::vec3(0.0f, 0.0f, 0.0f));
		depthShader.SetUniform("farPlane", 100.0f);
		for (int i = 0; i < 6; ++i)
		{
			depthShader.SetUniform("modelMat", modelModelMat[i]);
			ourModel.Render(depthShader);
		}
		cubeModelMat = glm::scale(e, glm::vec3(20.0f, 20.0f, 20.0f));
		depthShader.SetUniform("modelMat", cubeModelMat);
		cubeVAO.Bind();
		glDrawElements(GL_TRIANGLES, cubeIBO.GetCount(), GL_UNSIGNED_INT, nullptr);

		cubeDepthFBO.UnBind();
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Display DepthMap
		glDepthFunc(GL_LEQUAL);
		glViewport(0, 0, 600, 600);
		cubeDepthFBO.BindTexture(10);
		displayShader.Bind();
		displayShader.SetUniform("tex", 10);
		displayShader.SetUniform("pvMat", ourProjection.GetProjection() * glm::mat4(glm::mat3(ourView.GetView())));
		SkyboxRenderer::GetInstance().Render();

		// Display normal view
		glViewport(0, 600, 600, 600);
		glDepthFunc(GL_LESS);
		basicShader.Bind();
		basicShader.SetUniform("pvMat", ourProjection.GetProjection()* ourView.GetView());
		
		for (int i = 0; i < 6; ++i)
		{
			basicShader.SetUniform("modelMat", modelModelMat[i]);
			ourModel.Render(basicShader);
		}
		cubeModelMat = glm::scale(e, glm::vec3(20.0f, 20.0f, 20.0f));
		
		wood.Bind(5);
		basicShader.SetUniform("modelMat", cubeModelMat);
		basicShader.SetUniform("model.texture_diffuse0", 5);
		cubeVAO.Bind();
		glDrawElements(GL_TRIANGLES, cubeIBO.GetCount(), GL_UNSIGNED_INT, nullptr);

		// Display the simple shadow view

		//Display the test view
		glViewport(600, 0, 600, 600);
		testShader.Bind();
		cubeDepthFBO.BindTexture(20);
		testShader.SetUniform("pvMat", ourProjection.GetProjection()* ourView.GetView());
		testShader.SetUniform("lightPos", glm::vec3(0.0f, 0.0f, 0.0f));
		testShader.SetUniform("depthMap", 20);
		testShader.SetUniform("viewPos", ourView.GetPosition());
		testShader.SetUniform("farPlane", 100.0f);
		for (int i = 0; i < 6; ++i)
		{
			testShader.SetUniform("modelMat", modelModelMat[i]);
			testShader.SetUniform("normalMat", glm::mat3(glm::transpose(glm::inverse(modelModelMat[i]))));
			ourModel.Render(testShader);
		}
		cubeModelMat = glm::scale(e, glm::vec3(20.0f, 20.0f, 20.0f));
		testShader.SetUniform("modelMat", cubeModelMat);
		testShader.SetUniform("normalMat", glm::mat3(glm::transpose(glm::inverse(cubeModelMat))));
		cubeVAO.Bind();
		glDrawElements(GL_TRIANGLES, cubeIBO.GetCount(), GL_UNSIGNED_INT, nullptr);


		// test2
		glViewport(600, 600, 600, 600);
		shadowShader.Bind();
		/*
		uniform mat4 pvMat;
		uniform mat4 modelMat;
		uniform mat3 normalMat;
		uniform Model model;
		uniform vec3 lightPos;
		uniform samplerCube depthMap;
		uniform vec3 viewPos;
		uniform float farPlane;
		*/
		cubeDepthFBO.BindTexture(20);
		shadowShader.SetUniform("pvMat", ourProjection.GetProjection() * ourView.GetView());
		shadowShader.SetUniform("lightPos", glm::vec3(0.0f, 0.0f, 0.0f));
		shadowShader.SetUniform("depthMap", 20);
		shadowShader.SetUniform("viewPos", ourView.GetPosition());
		shadowShader.SetUniform("farPlane", 100.0f);
		for (int i = 0; i < 6; ++i)
		{
			shadowShader.SetUniform("modelMat", modelModelMat[i]);
			shadowShader.SetUniform("normalMat", glm::mat3(glm::transpose(glm::inverse(modelModelMat[i]))));
			ourModel.Render(shadowShader);
		}
		wood.Bind(3);
		pureWhite.Bind(4);
		cubeModelMat = glm::scale(e, glm::vec3(20.0f, 20.0f, 20.0f));
		shadowShader.SetUniform("modelMat", cubeModelMat);
		shadowShader.SetUniform("model.texture_diffuse0", 3);
		shadowShader.SetUniform("model.texture_specular0", 4);
		shadowShader.SetUniform("normalMat", glm::mat3(glm::transpose(glm::inverse(cubeModelMat))));
		cubeVAO.Bind();
		glDrawElements(GL_TRIANGLES, cubeIBO.GetCount(), GL_UNSIGNED_INT, nullptr);
		GET_ERROR;
		});
	int* x = new int(10);
	std::cout << *x << "\n";
	SingletonManager::Terminate();
	//Window::Terminate();
	_CrtDumpMemoryLeaks();
}

#endif