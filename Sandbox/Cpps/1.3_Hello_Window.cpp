#include "Entrypoint.h"
#if ACTIVE_LESSON_MAJOR == 1 && ACTIVE_LESSON_MINOR == 3 && TEST == 0

#include <iostream>
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "1.3_Hello_Window/Window.h"

int main()
{
	Firefly::Window windowInstance;
	GLFWwindow* window = windowInstance.GetWindow();

	glViewport(0, 0, 800, 800);
	glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
		});

	auto processInput = [](GLFWwindow* window) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		};

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		windowInstance.Render();
	}

	return 0;
}

#endif