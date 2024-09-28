#include "Entrypoint.h"
#if ACTIVE_LESSON_MAJOR == 4 && ACTIVE_LESSON_MINOR == 3 && TEST == 0

#include "Core.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

using namespace Firefly;
int main()
{
	Window::Init(1200, 800);
	Window& windowInstance = Window::GetInstance();
	EventManager::Init();
	SpriteRenderer::Init();
	ShaderLibrary::Init();

	Texture* texture = new Texture2D(ASSET("1.6_Texture/container.jpg"));
	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	glm::mat4 modelForframe(1.0f);
	//modelForframe = glm::scale(modelForframe, glm::vec3(2.0f, 2.0f, 2.0f));

	Shader& basicShader = ShaderLibrary::GetInstance().Get("BasicTexture2D");
	Shader& postShader = ShaderLibrary::GetInstance().Get("KernelTexture2D");
	Framebuffer fb(1200, 800);

	fb.Bind();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	fb.UnBind();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::mat3 kernel1 = glm::mat3(
		2.0f, 2.0f, 2.0f,
		2.0f, -15.0f, 2.0f,
		2.0f, 2.0f, 2.0f
	);
	glm::mat3 kernel2 = glm::mat3(
		1.0f / 16.0f, 1.0f / 16.0f, 1.0f / 16.0f,
		1.0f / 16.0f, 1.0f / 2.0f, 1.0f / 16.0f,
		1.0f / 16.0f, 1.0f / 16.0f, 1.0f / 16.0f
	);
	float offset = 1.0f / 300.0f;

	windowInstance.RenderLoop([&]() {
		fb.Bind();
		glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		basicShader.Bind();
		texture->Bind(0);
		glUniform1i(basicShader.GetLocation("tex"), 0);
		glUniformMatrix4fv(basicShader.GetLocation("model"), 1, GL_FALSE, &model[0][0]);
		SpriteRenderer::GetInstance().Render();

		fb.UnBind();
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		postShader.Bind();
		fb.BindTexture(2);
		glUniform1i(postShader.GetLocation("tex"), 2);
		glUniformMatrix4fv(postShader.GetLocation("model"), 1, GL_FALSE, &modelForframe[0][0]);
		glUniformMatrix3fv(postShader.GetLocation("kernel"), 1, GL_FALSE, &kernel2[0][0]);
		glUniform1f(postShader.GetLocation("offset"), offset);
		SpriteRenderer::GetInstance().Render();
		});

	SpriteRenderer::Terminate();
	ShaderLibrary::Terminate();
	Window::Terminate();
}

#endif