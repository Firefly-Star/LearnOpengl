#include "Entrypoint.h"
#if CHECK_ACTIVE(4, 6, 0)

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include "Core.h"

using namespace Firefly;
#if 1
int main()
{
	Window::Init(800, 800);
	Window& window = Window::GetInstance();
	EventManager::Init();
	ShaderLibrary::Init();
	SpriteRenderer::Init();
	SkyboxRenderer::Init();

	Shader ourShader({
	   {Shader::Type::VertexShader, ASSET("3.2_Model/model_vertex.glsl")},
	   {Shader::Type::FragmentShader, ASSET("3.2_Model/model_fragment.glsl")},
		});
	Shader& skyboxShader = ShaderLibrary::GetInstance().Get("Skybox3D");
	Shader& frameShader = ShaderLibrary::GetInstance().Get("GrayScaleTexture2D");
	Shader reflectShader({
		{Shader::Type::VertexShader, ASSET("4.4_Cube_Texture/reflect_vertex.glsl")},
		{Shader::Type::FragmentShader, ASSET("4.4_Cube_Texture/reflect_fragment.glsl")}
		});
	Shader refractShader({
		{Shader::Type::VertexShader, ASSET("4.4_Cube_Texture/reflect_vertex.glsl")},
		{Shader::Type::FragmentShader, ASSET("4.4_Cube_Texture/refract_fragment.glsl")}
		});

	Model ourModel(ASSET("3.2_Model/nanosuit/nanosuit.obj"));

	Texture* skybox = new TextureCube({
		ASSET("4.4_Cube_Texture/skybox/right.jpg"),
		ASSET("4.4_Cube_Texture/skybox/left.jpg"),
		ASSET("4.4_Cube_Texture/skybox/top.jpg"),
		ASSET("4.4_Cube_Texture/skybox/bottom.jpg"),
		ASSET("4.4_Cube_Texture/skybox/back.jpg"),
		ASSET("4.4_Cube_Texture/skybox/front.jpg"),
		});

	std::array<float, 4> params = { glm::radians(45.0f), 1.0f, 0.1f, 100.0f };
	Projection* projection = new PerspectiveProjection(params);
	View view;
	view.SetSpeed({ 3.0f, 3.0f, 3.0f, 0.5f });
	EventManager::GetInstance()->SubscribeKeyPress(std::bind(&View::SetCursorMode, &view, std::placeholders::_1));

	UniformBuffer ubo(4 * 4 * sizeof(float));
	ubo.Bind(1);
	skyboxShader.BindUniformBlock("Mat", 1);
	ourShader.BindUniformBlock("Mat", 1);
	reflectShader.BindUniformBlock("Mat", 1);
	refractShader.BindUniformBlock("Mat", 1);

	Firefly::FlashLight light;
	light.ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
	light.diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
	light.specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	light.innerCutOff = cos(glm::radians(12.5f));
	light.outerCutOff = cos(glm::radians(17.5f));

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	window.RenderLoop([&]() {
		Updater::Update();
		view.Update();
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Normal model with flashlight
#if 1
		ourShader.Bind();

		glm::mat4 pv = projection->GetProjection() * view.GetView();
		ubo.FillSub(0, sizeof(glm::mat4), &pv[0][0]);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, -5.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
		ourShader.SetUniform("modelMat", model);
		ourShader.SetUniform("normalMat", normalMat);

		ourShader.SetUniform("flashLight.ambientColor", light.ambientColor);
		ourShader.SetUniform("flashLight.diffuseColor", light.diffuseColor);
		ourShader.SetUniform("flashLight.specularColor", light.specularColor);
		ourShader.SetUniform("flashLight.innerCutOff", light.innerCutOff);
		ourShader.SetUniform("flashLight.outerCutOff", light.outerCutOff);
		ourShader.SetUniform("flashLight.position", view.GetPosition());
		ourShader.SetUniform("flashLight.direction", view.GetDirection());
		ourShader.SetUniform("observePos", view.GetPosition());
		ourModel.Render(ourShader);
#endif
		// model with reflection
#if 1
		reflectShader.Bind();

		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, glm::vec3(5.0f, -5.0f, -5.0f));
		model2 = glm::scale(model2, glm::vec3(0.5f, 0.5f, 0.5f));
		glm::mat3 normalMat2 = glm::inverse(glm::transpose(glm::mat3(model2)));
		reflectShader.SetUniform("modelMat", model2);
		reflectShader.SetUniform("normalMat", normalMat2);
		reflectShader.SetUniform("viewPos", view.GetPosition());
		skybox->Bind(2);
		reflectShader.SetUniform("tex", 2);
		ourModel.Render(reflectShader);
#endif
		// model with refraction
#if 1
		refractShader.Bind();

		glm::mat4 model3 = glm::mat4(1.0f);
		model3 = glm::translate(model3, glm::vec3(-5.0f, -5.0f, -5.0f));
		model3 = glm::scale(model3, glm::vec3(0.5f, 0.5f, 0.5f));
		glm::mat3 normalMat3 = glm::inverse(glm::transpose(glm::mat3(model3)));
		refractShader.SetUniform("modelMat", model3);
		refractShader.SetUniform("normalMat", normalMat3);
		refractShader.SetUniform("viewPos", view.GetPosition());
		skybox->Bind(2);
		refractShader.SetUniform("tex", 2);
		float ratio = sin(Time::GetTime()) * 0.25f + 0.5f;
		refractShader.SetUniform("ratio", ratio);
		ourModel.Render(refractShader);
#endif
		// Render Skybox
		skybox->Bind(2);
		skyboxShader.Bind();
		glm::mat4 pvMat = projection->GetProjection() * glm::mat4(glm::mat3(view.GetView()));
		skyboxShader.SetUniform("pvMat", pvMat);
		skyboxShader.SetUniform("tex", 2);
		SkyboxRenderer::GetInstance().Render();
		});

	delete skybox;
	SkyboxRenderer::Terminate();
	SpriteRenderer::Terminate();
	ShaderLibrary::Terminate();
	EventManager::Terminate();
	Window::Terminate();
}
#endif
#if 0
int main()
{
	Window::Init(800, 800);
	Window& window = Window::GetInstance();
	EventManager::Init();
	ShaderLibrary::Init();
	SpriteRenderer::Init();
	SkyboxRenderer::Init();

	Texture* skybox = new TextureCube({
		ASSET("4.4_Cube_Texture/skybox/right.jpg"),
		ASSET("4.4_Cube_Texture/skybox/left.jpg"),
		ASSET("4.4_Cube_Texture/skybox/top.jpg"),
		ASSET("4.4_Cube_Texture/skybox/bottom.jpg"),
		ASSET("4.4_Cube_Texture/skybox/back.jpg"),
		ASSET("4.4_Cube_Texture/skybox/front.jpg"),
		});

	Shader& skyboxShader = ShaderLibrary::GetInstance().Get("Skybox3D");

	std::array<float, 4> params = { glm::radians(45.0f), 1.0f, 0.1f, 100.0f };
	Projection* projection = new PerspectiveProjection(params);
	View view;
	view.SetSpeed({ 3.0f, 3.0f, 3.0f, 0.5f });
	EventManager::GetInstance()->SubscribeKeyPress(std::bind(&View::SetCursorMode, &view, std::placeholders::_1));
	glm::mat4 pvMat = projection->GetProjection() * glm::mat4(glm::mat3(view.GetView()));

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	UniformBuffer ubo(sizeof(glm::mat4));
	ubo.Bind(2);

	skyboxShader.BindUniformBlock("Mat", 2);

	window.RenderLoop([&]() {
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Updater::Update();
		view.Update();
		pvMat = projection->GetProjection() * glm::mat4(glm::mat3(view.GetView()));
		ubo.FillSub(0, sizeof(glm::mat4), &pvMat[0][0]);
		skybox->Bind(2);
		skyboxShader.Bind();
		glUniformMatrix4fv(skyboxShader.GetLocation("pvMat"), 1, GL_FALSE, &pvMat[0][0]);
		glUniform1i(skyboxShader.GetLocation("tex"), 2);
		SkyboxRenderer::GetInstance().Render();

		});

	delete skybox;
	delete projection;
	SkyboxRenderer::Terminate();
	SpriteRenderer::Terminate();
	ShaderLibrary::Terminate();
	EventManager::Terminate();
	Window::Terminate();
}
#endif

#endif