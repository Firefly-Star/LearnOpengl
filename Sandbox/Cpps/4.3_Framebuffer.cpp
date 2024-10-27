#include "Entrypoint.h"
#if CHECK_ACTIVE(4, 3, 0)

#include "Core.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

using namespace Firefly;
int main()
{
	Window::Init(1200, 800);
	Window& windowInstance = Window::GetInstance();
	BindManager::Init();
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
		basicShader.SetUniform("tex", 0);
		basicShader.SetUniform("model", model);
		SpriteRenderer::GetInstance().Render();


		fb.UnBind();
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		fb.DirectRender();
		//postShader.Bind();
		//fb.BindTexture(2);
		//postShader.SetUniform("tex", 2);
		//postShader.SetUniform("model", modelForframe);
		//postShader.SetUniform("kernel", kernel2);
		//postShader.SetUniform("offset", offset);
		//SpriteRenderer::GetInstance().Render();
		});

	SpriteRenderer::Terminate();
	ShaderLibrary::Terminate();
	Window::Terminate();
}

#endif