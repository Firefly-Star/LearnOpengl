#include "Entrypoint.h"
#if ACTIVE_LESSON_MAJOR == 2 && ACTIVE_LESSON_MINOR == 1 && TEST == 0

#include "0.0_Extentions/Updater/Updater.h"
#include "0.0_Extentions/IOSystem/EventSystem/EventManager.h"
#include "1.3_Hello_Window/Window.h"
#include "1.4_Hello_Triangle/IndexBuffer.h"
#include "1.4_Hello_Triangle/Shader.h"
#include "1.4_Hello_Triangle/VertexArray.h"
#include "1.4_Hello_Triangle/VertexBuffer.h"
#include "1.6_Texture/Texture.h"
#include "1.9_Camera/Projection.h"
#include "1.9_Camera/View.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/string_cast.hpp"
#include <iostream>

using namespace Firefly;

int main()
{
	Window::Init(1200, 960);
	Window* windowInstance = Window::GetInstance();
	EventManager::Init();

    VertexArray vao;
    vao.Bind();
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    VertexBuffer vbo(vertices, sizeof(vertices));
    vbo.Bind();
    vao.SetLayout({
        {VertexArray::Type::Float, 3},
        {VertexArray::Type::Float, 3}
        });
    
    Shader shader({
        {Shader::Type::VertexShader, ASSET("2.1_Basic_Lighting/basiclight_vertex.glsl")},
        {Shader::Type::FragmentShader, ASSET("2.1_Basic_Lighting/basiclight_fragment.glsl")}
        });
    shader.Bind();
    
    glm::mat4 E(1.0f);
    //glm::mat4 model = glm::rotate(E, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(E, glm::vec3(1.0f, 1.0f, 2.0f));
    glm::mat4 model(1.0f);
    std::array<float, 4> param{ 45.0f, 1200.0f / 960.0f, 0.1f, 100.0f };
    Projection* projection = new PerspectiveProjection(param);
    View view(glm::vec3(0.0f, 0.0f, 5.0f));
    view.SetSpeed({ 1.0f, 1.0f, 1.0f, 1.0f });

    EventManager::GetInstance()->SubscribeKeyPress(std::bind(&View::SetCursorMode, &view, std::placeholders::_1));

    glEnable(GL_DEPTH_TEST);

	windowInstance->RenderLoop([&]() {
        Updater::Update();
        view.Update();

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 pv = projection->GetProjection() * view.GetView();
        glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(model)));
        const glm::vec3 position = view.GetPosition();
        glUniformMatrix4fv(shader.GetLocation("pv"), 1, GL_FALSE, &pv[0][0]);
        glUniformMatrix4fv(shader.GetLocation("model"), 1, GL_FALSE, &model[0][0]);
        glUniform3f(shader.GetLocation("lightColor"), 0.4f, 0.2f, 0.4f);
        glUniform3f(shader.GetLocation("lightPos"), 1.0f, 3.0f, 3.0f);
        glUniform3f(shader.GetLocation("observerPos"), position.x, position.y, position.z);
        glUniformMatrix3fv(shader.GetLocation("normalMat"), 1, GL_FALSE, &normalMat[0][0]);

        glDrawArrays(GL_TRIANGLES, 0, 36);
		});

	EventManager::Terminate();
	Window::Terminate();
}

#endif