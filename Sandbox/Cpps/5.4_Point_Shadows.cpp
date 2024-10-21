#include "Entrypoint.h"
#if CHECK_ACTIVE(5, 4, 0)

#include "Core.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <gtx/string_cast.hpp>

using namespace Firefly;

int main()
{
	Window::Init(1200, 1200);
	SingletonManager::Init();

	Window::GetInstance().RenderLoop([]() {
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		});

	SingletonManager::Terminate();
}

#endif