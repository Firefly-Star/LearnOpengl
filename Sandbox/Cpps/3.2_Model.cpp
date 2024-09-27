#include "Entrypoint.h"
#if ACTIVE_LESSON_MAJOR == 3 && ACTIVE_LESSON_MINOR == 2 && TEST == 0

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Core.h"

#include <iostream>

int main()
{
    Firefly::Window::Init(1200, 1200);
    Firefly::EventManager::Init();

    Firefly::Window::GetInstance().Enable(Firefly::RenderStatus::DepthTest);

    Firefly::Shader ourShader({
        {Firefly::Shader::Type::VertexShader, ASSET("3.2_Model/model_vertex.glsl")},
        {Firefly::Shader::Type::FragmentShader, ASSET("3.2_Model/model_fragment.glsl")},
        });

    Firefly::Model ourModel(ASSET("3.2_Model/nanosuit/nanosuit.obj"));

    std::array<float, 4> param = { glm::radians(45.0f), 1.0f, 0.1f, 100.0f };
    Firefly::Projection* projection = new Firefly::PerspectiveProjection(param);
    Firefly::View view(glm::vec3(0.0f, 5.0f, 10.0f));
    view.SetSpeed({ 3.0f, 3.0f, 3.0f, 1.0f });

    Firefly::FlashLight light;
    light.ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
    light.diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
    light.specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
    light.innerCutOff = cos(glm::radians(12.5f));
    light.outerCutOff = cos(glm::radians(17.5f));

    Firefly::EventManager::GetInstance()->SubscribeKeyPress(std::bind(&Firefly::View::SetCursorMode, &view, std::placeholders::_1));

    Firefly::Window::GetInstance().RenderLoop([&]() {
        Firefly::Updater::Update();
        view.Update();

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

        ourShader.Bind();

        glm::mat4 pv = projection->GetProjection() * view.GetView();

        glUniformMatrix4fv(ourShader.GetLocation("pvMat"), 1, GL_FALSE, &pv[0][0]);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
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
        });
    return 0;
}

#endif