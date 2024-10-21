#include "Entrypoint.h"
#if CHECK_ACTIVE(5, 3, 0)

#include "Core.h"
using namespace Firefly;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main()
{
	Window::Init(1600, 1600);
	auto& windowInstance = Window::GetInstance();
	SingletonManager::Init();

	Shader depthShader({
		{Shader::Type::VertexShader, ASSET("5.3_ShadowMapping/depth_vertex.glsl")},
		{Shader::Type::FragmentShader, ASSET("5.3_ShadowMapping/depth_fragment.glsl")}
		});
	//Shader displayShader({
	//	{Shader::Type::VertexShader, ASSET("5.3_ShadowMapping/display_vertex.glsl")},
	//	{Shader::Type::FragmentShader, ASSET("5.3_ShadowMapping/display_fragment.glsl")}
	//	});
	Shader displayShader({
		{Shader::Type::VertexShader, ASSET("5.3_ShadowMapping/display_vertex.glsl")},
		{Shader::Type::FragmentShader, ASSET("5.3_ShadowMapping/display_perspective_fragment.glsl")}
		});
	Shader originShader({
		{Shader::Type::VertexShader, ASSET("5.3_ShadowMapping/origin_vertex.glsl")},
		{Shader::Type::FragmentShader, ASSET("5.3_ShadowMapping/origin_fragment.glsl")}
		});
	//Shader shadowShader({
	//	{Shader::Type::VertexShader, ASSET("5.3_ShadowMapping/shadow_vertex.glsl")},
	//	{Shader::Type::FragmentShader, ASSET("5.3_ShadowMapping/shadow_fragment.glsl")}
	//	});
	Shader shadowShader({
		{Shader::Type::VertexShader, ASSET("5.3_ShadowMapping/shadow_vertex.glsl")},
		{Shader::Type::FragmentShader, ASSET("5.3_ShadowMapping/shadow_perspective_fragment.glsl")}
		});
	Shader basicShader({
		{Shader::Type::VertexShader, ASSET("5.3_ShadowMapping/shadow_vertex.glsl")},
		{Shader::Type::FragmentShader, ASSET("5.3_ShadowMapping/basic_fragment.glsl")}
		});
	//Shader testShader({
	//	{Shader::Type::VertexShader, ASSET("5.3_ShadowMapping/shadow_vertex.glsl")},
	//	{Shader::Type::FragmentShader, ASSET("5.3_ShadowMapping/temp.glsl")}
	//	});
	Shader testShader({
		{Shader::Type::VertexShader, ASSET("5.3_ShadowMapping/shadow_vertex.glsl")},
		{Shader::Type::FragmentShader, ASSET("5.3_ShadowMapping/temp_perspective.glsl")}
		});

	float bottomSize = 40.0f;
	float cubeVertices[] = {
		-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,

		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,

		-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		 1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		 1.0f,  1.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 
		 1.0f,  1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f,  1.0f,  1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,

		 1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,
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
	float bottomVertices[] = {
		-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, bottomSize,
		 1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, bottomSize, bottomSize,
		 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, bottomSize, 0.0f
	};
	unsigned int bottomIndices[] = {
		0, 1, 2,
		2, 3, 0
	};

	VertexArray cubeVAO;
	cubeVAO.Bind();
	VertexBuffer cubeVBO(cubeVertices, sizeof(cubeVertices));
	cubeVAO.SetLayout({
		{
			{
				{VertexArray::Type::Float, 3},
				{VertexArray::Type::Float, 3},
				{VertexArray::Type::Float, 2}
			}, cubeVBO
		}
		});
	IndexBuffer cubeIBO(cubeIndices, sizeof(cubeIndices));
	cubeIBO.Bind();
	cubeVAO.UnBind();

	VertexArray bottomVAO;
	bottomVAO.Bind();
	VertexBuffer bottomVBO(bottomVertices, sizeof(bottomVertices));
	bottomVAO.SetLayout({
		{
			{
				{VertexArray::Type::Float, 3},
				{VertexArray::Type::Float, 3},
				{VertexArray::Type::Float, 2}
			}, bottomVBO
		}
		});
	IndexBuffer bottomIBO(bottomIndices, sizeof(bottomIndices));
	bottomIBO.Bind();
	bottomVAO.UnBind();

	Model ourmodel(ASSET("3.2_Model/nanosuit/nanosuit.obj"));

	int shadowWidth = 1000;
	int shadowHeight = 1000;
	DepthFramebuffer depthMapFBO(shadowWidth, shadowHeight);

	//auto lightProjection = Projection::Create<Projection::Type::Ortho>(std::array<float, 6>{ -10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 20.0f });
	auto lightProjection = Projection::Create<Projection::Type::Perspective>(std::array<float, 4>{glm::radians(45.0f), 1.0f, 0.1f, 100.0f});
	View lightView(glm::vec3(20.0f, 20.0f, 20.0f), glm::vec3(-1.0f, -1.0f, -1.0f));
	auto normalProjection = Projection::Create<Projection::Type::Perspective>(std::array<float, 4>{glm::radians(45.0f), 1.0f, 0.1f, 100.0f});
	lightView.SetSpeed();

	View normalView(glm::vec3(2.0f, 0.0f, 0.0f));
	normalView.SetSpeed({5.0f, 5.0f, 5.0f, 1.0f});

	Texture* woodTexture = new Texture2D(ASSET("5.3_ShadowMapping/wood.png"));
	Texture* wallTexture = new Texture2D(ASSET("1.6_Texture/wall.jpg"));

	depthMapFBO.Bind();
	glEnable(GL_DEPTH_TEST);
	depthMapFBO.UnBind();
	glEnable(GL_DEPTH_TEST);

	glm::mat4 cubeModelMat = glm::mat4(1.0f);
	glm::mat4 bottomModelMat = glm::scale(glm::mat4(1.0f), glm::vec3(bottomSize, 1.0f, bottomSize));
	glm::mat4 modelModelMat = 
		glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, -2.0f)) * 
		glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));

	FrameRatio fps;

	windowInstance.RenderLoop([&]() {
		Updater::Update();
		//lightView.Update();
		normalView.Update();
		fps.Update();
		
		// Gen depthMap
		depthMapFBO.Bind();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, shadowWidth, shadowHeight);
		
		depthShader.Bind();
		depthShader.SetUniform("pvMat", lightProjection->GetProjection() * lightView.GetView());
		depthShader.SetUniform("modelMat", cubeModelMat);
		cubeVAO.Bind();
		glDrawElements(GL_TRIANGLES, cubeIBO.GetCount(), GL_UNSIGNED_INT, nullptr);

		depthShader.SetUniform("modelMat", modelModelMat);
		ourmodel.Render(depthShader);

		depthShader.SetUniform("modelMat", bottomModelMat);
		bottomVAO.Bind();
		glDrawElements(GL_TRIANGLES, bottomIBO.GetCount(), GL_UNSIGNED_INT, nullptr);

		// Display depthMap
		depthMapFBO.UnBind();
		glViewport(0, 0, 800, 800);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		depthMapFBO.BindTexture(2);
		displayShader.Bind();
		displayShader.SetUniform("tex", 2);
		displayShader.SetUniform("model", glm::mat4(1.0f));
		SpriteRenderer::GetInstance().Render();
#if 0
		// Display the light "view"
		glViewport(800, 0, 800, 800);
		originShader.Bind();
		originShader.SetUniform("pvMat", normalProjection->GetProjection() * lightView.GetView());
		originShader.SetUniform("modelMat", cubeModelMat);
		cubeVAO.Bind();
		glDrawElements(GL_TRIANGLES, cubeIBO.GetCount(), GL_UNSIGNED_INT, nullptr);

		originShader.SetUniform("modelMat", bottomModelMat);
		bottomVAO.Bind();
		glDrawElements(GL_TRIANGLES, bottomIBO.GetCount(), GL_UNSIGNED_INT, nullptr);
#endif
		// Display the normal view with simple shadow
		glViewport(800, 0, 800, 800);
		depthMapFBO.BindTexture(2);
		testShader.Bind();
		testShader.SetUniform("depthMap", 2);
		testShader.SetUniform("pvMat", normalProjection->GetProjection()* normalView.GetView());
		testShader.SetUniform("modelMat", cubeModelMat);
		testShader.SetUniform("normalMat", glm::mat4(1.0f));
		testShader.SetUniform("lightpvmMat", lightProjection->GetProjection() * lightView.GetView() * cubeModelMat);
		cubeVAO.Bind();
		glDrawElements(GL_TRIANGLES, cubeIBO.GetCount(), GL_UNSIGNED_INT, nullptr);

		testShader.SetUniform("modelMat", bottomModelMat);
		testShader.SetUniform("lightpvmMat", lightProjection->GetProjection()* lightView.GetView() * bottomModelMat);
		bottomVAO.Bind();
		glDrawElements(GL_TRIANGLES, bottomIBO.GetCount(), GL_UNSIGNED_INT, nullptr);

		testShader.SetUniform("modelMat", modelModelMat);
		testShader.SetUniform("lightpvmMat", lightProjection->GetProjection()* lightView.GetView()* modelModelMat);
		ourmodel.Render(testShader);

		// Display the normal view
		glViewport(0, 800, 800, 800);
		basicShader.Bind();
		woodTexture->Bind(3);
		basicShader.SetUniform("pvMat", normalProjection->GetProjection()* normalView.GetView());
		basicShader.SetUniform("modelMat", cubeModelMat);
		basicShader.SetUniform("normalMat", glm::mat4(1.0f));
		basicShader.SetUniform("lightpvmMat", normalProjection->GetProjection()* lightView.GetView());
		basicShader.SetUniform("model.texture_diffuse0", 3);
		cubeVAO.Bind();
		glDrawElements(GL_TRIANGLES, cubeIBO.GetCount(), GL_UNSIGNED_INT, nullptr);

		basicShader.SetUniform("modelMat", bottomModelMat);
		bottomVAO.Bind();
		glDrawElements(GL_TRIANGLES, bottomIBO.GetCount(), GL_UNSIGNED_INT, nullptr);

		basicShader.SetUniform("modelMat", modelModelMat);
		ourmodel.Render(basicShader);

		// Display the normal view with simple shadow
		glViewport(800, 800, 800, 800);
		shadowShader.Bind();
		woodTexture->Bind(3);
		depthMapFBO.BindTexture(2);
		shadowShader.SetUniform("depthMap", 2);
		shadowShader.SetUniform("model.texture_diffuse0", 3);
		shadowShader.SetUniform("pvMat", normalProjection->GetProjection() * normalView.GetView());
		shadowShader.SetUniform("modelMat", cubeModelMat);
		shadowShader.SetUniform("normalMat", glm::mat3(1.0f));
		shadowShader.SetUniform("viewPos", normalView.GetPosition());
		shadowShader.SetUniform("lightDir", glm::vec3(-1.0f, -1.0f, -1.0f));
		shadowShader.SetUniform("lightpvmMat", lightProjection->GetProjection()* lightView.GetView() * cubeModelMat);
		cubeVAO.Bind();
		glDrawElements(GL_TRIANGLES, cubeIBO.GetCount(), GL_UNSIGNED_INT, nullptr);

		shadowShader.SetUniform("modelMat", bottomModelMat);
		shadowShader.SetUniform("lightpvmMat", lightProjection->GetProjection()* lightView.GetView() * bottomModelMat);
		bottomVAO.Bind();
		glDrawElements(GL_TRIANGLES, bottomIBO.GetCount(), GL_UNSIGNED_INT, nullptr);

		shadowShader.SetUniform("modelMat", modelModelMat);
		shadowShader.SetUniform("lightpvmMat", lightProjection->GetProjection()* lightView.GetView()* modelModelMat);
		ourmodel.Render(shadowShader);

		glViewport(0, 0, 1600, 1600);
		AxisRenderer::GetInstance().Render(normalView, 1000.0f, 1000.0f, 200.0f);
		});
	SingletonManager::Terminate();
}

#endif