#include "Entrypoint.h"
#if ACTIVE_LESSON_MAJOR == 4 && ACTIVE_LESSON_MINOR == 3 && TEST == 0

#include "0.0_Extentions/IOSystem/EventSystem/EventManager.h"
#include "1.3_Hello_Window/Window.h"
#include "1.4_Hello_Triangle/Shader.h"
#include "1.6_Texture/Texture.h"
#include "4.3_Framebuffer/SpriteRenderer.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

using namespace Firefly;
int main()
{
	Window::Init(1200, 800);
	Window& windowInstance = Window::GetInstance();
	EventManager::Init();
	SpriteRenderer::Init();
	Shader shader({
		{Shader::Type::VertexShader, ASSET("4.3_Framebuffer/pure_vertex.glsl")},
		{Shader::Type::FragmentShader, ASSET("4.3_Framebuffer/pure_fragment.glsl")}
		});

	Texture* texture = new Texture2D(ASSET("1.6_Texture/container.jpg"));
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));

	windowInstance.RenderLoop([&]() {
		shader.Bind();
		texture->Bind(0);
		glUniform1i(shader.GetLocation("tex"), 0);
		glUniformMatrix4fv(shader.GetLocation("model"), 1, GL_FALSE, &model[0][0]);
		SpriteRenderer::GetInstance().Render();
		});

	SpriteRenderer::Terminate();
}

#endif