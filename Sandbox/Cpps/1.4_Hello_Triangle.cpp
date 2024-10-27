#include "Entrypoint.h"
#if CHECK_ACTIVE(1, 4, 0)

#include <1.3_Hello_Window/Window.h>
#include <1.4_Hello_Triangle/Shader.h>
#include <1.4_Hello_Triangle/VertexBuffer.h>
#include <1.4_Hello_Triangle/IndexBuffer.h>
#include <1.4_Hello_Triangle/VertexArray.h>
#include <iostream>

void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    Firefly::Window windowInstance;
    GLFWwindow* window = windowInstance.GetWindow();

    std::vector<Firefly::Shader::File> files =
    {
        {Firefly::Shader::Type::VertexShader, "C:/Users/Summer/Desktop/OpenGL/OpenGL/src/assets/1.4_Hello_Triangle/simple_vertex.glsl"},
        {Firefly::Shader::Type::FragmentShader, "C:/Users/Summer/Desktop/OpenGL/OpenGL/src/assets/1.4_Hello_Triangle/simple_fragment.glsl"}
    };
    Firefly::Shader shader(files);
    shader.Bind();
    
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f   
    };
    Firefly::VertexBuffer vbo(vertices, sizeof(vertices));
    vbo.Bind();

    Firefly::VertexArray vao;
    vao.SetLayout({ {Firefly::VertexArray::Type::Float, 3}, {Firefly::VertexArray::Type::Float, 3} });
    vao.Bind();

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    Firefly::IndexBuffer ibo(indices, sizeof(indices));
    ibo.Bind();

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

#endif