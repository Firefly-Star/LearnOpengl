#include "Entrypoint.h"
#if ACTIVE_LESSON_MAJOR == 2 && ACTIVE_LESSON_MINOR == 4 && TEST == 0

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
#include "2.2_Material/Material.h"
#include "2.4_Light_Casters/LightCasters.h"

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
        {Shader::Type::VertexShader, ASSET("2.2_Material/material_vertex.glsl")},
        {Shader::Type::FragmentShader, ASSET("2.4_Light_Casters/flashLight_fragment.glsl")}
        });
    shader.Bind();

    glm::mat4 E(1.0f);
    glm::mat4 model(1.0f);
    std::array<float, 4> param{ 45.0f, 1200.0f / 960.0f, 0.1f, 100.0f };
    Projection* projection = new PerspectiveProjection(param);
    View view(glm::vec3(0.0f, 0.0f, 5.0f));
    view.SetSpeed({ 1.0f, 1.0f, 1.0f, 1.0f });

    EventManager::GetInstance()->SubscribeKeyPress(std::bind(&View::SetCursorMode, &view, std::placeholders::_1));

    Material material;
    material.ambientColor = glm::vec3(0.0215, 0.1745, 0.0215);
    material.diffuseColor = glm::vec3(0.07568, 0.61424, 0.07568);
    material.specularColor = glm::vec3(0.633, 0.727811, 0.633);
    material.shininess = 0.6f * 128;

    /*DirectionalLight light;
    light.direction = glm::vec3(-1.0f, -1.0f, -0.5f);
    light.ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
    light.diffuseColor = glm::vec3(0.2f, 0.2f, 0.2f);
    light.specularColor = glm::vec3(1.0f, 1.0f, 1.0f);*/

    /*SpotLight light;
    light.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
    light.specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
    light.constant = 1.0f;
    light.linear = 0.7f;
    light.quadratic = 1.8f;*/

    FlashLight light;
    light.ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
    light.diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
    light.specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
    light.innerCutOff = 12.5f;
    light.outerCutOff = 17.5f;

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
        glUniform3f(shader.GetLocation("observerPos"), position.x, position.y, position.z);
        glUniformMatrix3fv(shader.GetLocation("normalMat"), 1, GL_FALSE, &normalMat[0][0]);

        glUniform3f(shader.GetLocation("material.ambientColor"), material.ambientColor.x, material.ambientColor.y, material.ambientColor.z);
        glUniform3f(shader.GetLocation("material.diffuseColor"), material.diffuseColor.x, material.diffuseColor.y, material.diffuseColor.z);
        glUniform3f(shader.GetLocation("material.specularColor"), material.specularColor.x, material.specularColor.y, material.specularColor.z);
        glUniform1f(shader.GetLocation("material.shininess"), material.shininess);

        //light.direction = glm::vec3(sin(glfwGetTime()), 1.0f, cos(glfwGetTime()));
        light.position = view.GetPosition();
        light.direction = view.GetDirection();
        glUniform3f(shader.GetLocation("light.position"), light.position.x, light.position.y, light.position.z);
        glUniform3f(shader.GetLocation("light.direction"), light.direction.x, light.direction.y, light.direction.z);
        glUniform3f(shader.GetLocation("light.ambientColor"), light.ambientColor.x, light.ambientColor.y, light.ambientColor.z);
        glUniform3f(shader.GetLocation("light.diffuseColor"), light.diffuseColor.x, light.diffuseColor.y, light.diffuseColor.z);
        glUniform3f(shader.GetLocation("light.specularColor"), light.specularColor.x, light.specularColor.y, light.specularColor.z);
        glUniform1f(shader.GetLocation("light.innerCutOff"), cos(glm::radians(light.innerCutOff)));
        glUniform1f(shader.GetLocation("light.outerCutOff"), cos(glm::radians(light.outerCutOff)));

        glDrawArrays(GL_TRIANGLES, 0, 36);
        });

    EventManager::Terminate();
    Window::Terminate();
}

#endif