#include "Entrypoint.h"
#if ACTIVE_LESSON_MAJOR == 4 && ACTIVE_LESSON_MINOR == 4 && TEST == 0

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include "Core.h"

using namespace Firefly;

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

	Model ourModel(ASSET("3.2_Model/nanosuit/nanosuit.obj"));

	Texture* skybox = new TextureCube({
		ASSET("4.4_Cube_Texture/skybox/right.jpg"),
		ASSET("4.4_Cube_Texture/skybox/left.jpg"),
		ASSET("4.4_Cube_Texture/skybox/top.jpg"),
		ASSET("4.4_Cube_Texture/skybox/bottom.jpg"),
		ASSET("4.4_Cube_Texture/skybox/back.jpg"),
		ASSET("4.4_Cube_Texture/skybox/front.jpg"),
		});

	Shader& skyboxShader = ShaderLibrary::GetInstance().Get("Skybox3D");
	Shader& frameShader = ShaderLibrary::GetInstance().Get("GrayScaleTexture2D");

	std::array<float, 4> params = {glm::radians(45.0f), 1.0f, 0.1f, 100.0f};
	Projection* projection = new PerspectiveProjection(params);
	View view;
	view.SetSpeed({3.0f, 3.0f, 3.0f, 1.0f});
	EventManager::GetInstance()->SubscribeKeyPress(std::bind(&View::SetCursorMode, &view, std::placeholders::_1));

	Firefly::FlashLight light;
	light.ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
	light.diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
	light.specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	light.innerCutOff = cos(glm::radians(12.5f));
	light.outerCutOff = cos(glm::radians(17.5f));

	Framebuffer fb(800, 800);
	fb.Bind();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	fb.UnBind();
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	window.RenderLoop([&]() {
		Updater::Update();
		view.Update();
		// Render in the framebuffer
		fb.Bind();
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Render model First
		ourShader.Bind();

		glm::mat4 pv = projection->GetProjection() * view.GetView();

		glUniformMatrix4fv(ourShader.GetLocation("pvMat"), 1, GL_FALSE, &pv[0][0]);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, -5.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(ourShader.GetLocation("modelMat"), 1, GL_FALSE, &model[0][0]);
		ourModel.Render(ourShader);
		glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
		glUniformMatrix3fv(ourShader.GetLocation("normalMat"), 1, GL_FALSE, &normalMat[0][0]);

		glUniform3fv(ourShader.GetLocation("flashLight.ambientColor"), 1, &light.ambientColor[0]);
		glUniform3fv(ourShader.GetLocation("flashLight.diffuseColor"), 1, &light.diffuseColor[0]);
		glUniform3fv(ourShader.GetLocation("flashLight.specularColor"), 1, &light.specularColor[0]);
		glUniform1f(ourShader.GetLocation("flashLight.innerCutOff"), light.innerCutOff);
		glUniform1f(ourShader.GetLocation("flashLight.outerCutOff"), light.outerCutOff);

		glUniform3fv(ourShader.GetLocation("flashLight.position"), 1, &(view.GetPosition()[0]));
		glUniform3fv(ourShader.GetLocation("flashLight.direction"), 1, &(view.GetDirection()[0]));

		glUniform3fv(ourShader.GetLocation("observePos"), 1, &(view.GetPosition()[0]));

		// Render Skybox
		skybox->Bind(2);
		skyboxShader.Bind();
		glm::mat4 pvMat = projection->GetProjection() * glm::mat4(glm::mat3(view.GetView()));
		glUniformMatrix4fv(skyboxShader.GetLocation("pvMat"), 1, GL_FALSE, &pvMat[0][0]);
		glUniform1i(skyboxShader.GetLocation("tex"), 2);
		SkyboxRenderer::GetInstance().Render();

		// Render with Framebuffer
		fb.UnBind();
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		fb.BindTexture(2);
		frameShader.Bind();
		glm::mat4 modelFrame(1.0f);
		glUniformMatrix4fv(frameShader.GetLocation("model"), 1, GL_FALSE, &modelFrame[0][0]);
		glUniform1i(frameShader.GetLocation("tex"), 2);
		SpriteRenderer::GetInstance().Render();
		});

	delete skybox;
	SkyboxRenderer::Terminate();
	SpriteRenderer::Terminate();
	ShaderLibrary::Terminate();
	EventManager::Terminate();
	Window::Terminate();
}

#endif