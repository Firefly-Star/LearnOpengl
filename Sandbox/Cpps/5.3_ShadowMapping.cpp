#include "Entrypoint.h"
#if CHECK_ACTIVE(5, 3, 0)

#include "Core.h"
using namespace Firefly;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main()
{
	Window::Init(1000, 1000);
	Window& windowInstance = Window::GetInstance();
	BindManager::Init();
	EventManager::Init();

	windowInstance.RenderLoop([&]() {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		});

}

#endif