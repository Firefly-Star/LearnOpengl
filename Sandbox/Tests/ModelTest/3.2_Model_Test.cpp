#include "Entrypoint.h"
#if ACTIVE_LESSON_MAJOR == 3 && ACTIVE_LESSON_MINOR == 2 && TEST == 1

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Model.h"
#include "0.0_Extentions/IOSystem/EventSystem/EventManager.h"
#include "0.0_Extentions/Updater/Updater.h"
#include "1.3_Hello_Window/Window.h"
#include "1.4_Hello_Triangle/Shader.h"
#include "1.9_Camera/Projection.h"
#include "1.9_Camera/View.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/string_cast.hpp>

#include <iostream>

int main()
{
    Firefly::Window::Init(1200, 1200);
    Firefly::Window* windowInstance = Firefly::Window::GetInstance();
    Firefly::EventManager::Init();
    
    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);

    Firefly::Shader ourShader({
        {Firefly::Shader::Type::VertexShader, ASSET("2.2_Material/material_vertex.glsl")},
        {Firefly::Shader::Type::FragmentShader, ASSET("3.2_Model/test_fragment.glsl")},
        });

    Model ourModel("C:/Users/Summer/Desktop/OpenGL/OpenGL/src/assets/3.2_Model/nanosuit/nanosuit.obj");

    std::array<float, 4> param = { glm::radians(45.0f), 1.0f, 0.1f, 100.0f };
    Firefly::Projection* projection = new Firefly::PerspectiveProjection(param);
    Firefly::View view(glm::vec3(0.0f, 5.0f, 10.0f));
    view.SetSpeed({ 3.0f, 3.0f, 3.0f, 1.0f });

    Firefly::EventManager::GetInstance()->SubscribeKeyPress(std::bind(&Firefly::View::SetCursorMode, &view, std::placeholders::_1));

    windowInstance->RenderLoop([&]() {
        Firefly::Updater::Update();
        view.Update();

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.Bind();

        // view/projection transformations
        glm::mat4 pv = projection->GetProjection() * view.GetView();

        glUniformMatrix4fv(ourShader.GetLocation("pv"), 1, GL_FALSE, &pv[0][0]);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        glUniformMatrix4fv(ourShader.GetLocation("model"), 1, GL_FALSE, &model[0][0]);
        ourModel.Draw(ourShader);
        });
    return 0;
}

#endif