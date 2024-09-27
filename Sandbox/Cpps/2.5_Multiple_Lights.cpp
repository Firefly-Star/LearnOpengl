#include "Entrypoint.h"
#if ACTIVE_LESSON_MAJOR == 2 && ACTIVE_LESSON_MINOR == 5 && TEST == 0

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
#include <assimp/Importer.hpp>

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
    Shader shader({
        {Shader::Type::VertexShader, ASSET("2.2_Material/material_vertex.glsl")},
        {Shader::Type::FragmentShader, ASSET("2.5_Multiple_Lights/multipleLights_fragment.glsl")}
        });
    {
        vbo.Bind();
        vao.SetLayout({
            {VertexArray::Type::Float, 3},
            {VertexArray::Type::Float, 3}
            });
        shader.Bind();
    }

    glm::mat4 E(1.0f);
    glm::mat4 model(1.0f);
    std::array<float, 4> param{ 45.0f, 1200.0f / 960.0f, 0.1f, 100.0f };
    Projection* projection = new PerspectiveProjection(param);
    View view(glm::vec3(0.0f, 0.0f, 5.0f));
    view.SetSpeed({ 1.0f, 1.0f, 1.0f, 1.0f });

    EventManager::GetInstance()->SubscribeKeyPress(std::bind(&View::SetCursorMode, &view, std::placeholders::_1));

    Material material;
    {
        material.ambientColor = glm::vec3(0.0215, 0.1745, 0.0215);
        material.diffuseColor = glm::vec3(0.07568, 0.61424, 0.07568);
        material.specularColor = glm::vec3(0.633, 0.727811, 0.633);
        material.shininess = 0.6f * 128;
    }

    DirectionalLight directionalLight;
    {
        directionalLight.direction = glm::vec3(-1.0f, -1.0f, -0.5f);
        directionalLight.ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
        directionalLight.diffuseColor = glm::vec3(0.2f, 0.2f, 0.2f);
        directionalLight.specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
    }

    SpotLight spotLight[4];
    for (int i = 0; i < 4; ++i)
    {
        const float pi = 3.1415926535f;
        spotLight[i].position = glm::vec3(2 * sin(i / 4 * pi), 2 * cos(i / 4 * pi), 1.0f);
        spotLight[i].ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
        spotLight[i].diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
        spotLight[i].specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
        spotLight[i].constant = 1.0f;
        spotLight[i].linear = 0.7f;
        spotLight[i].quadratic = 1.8f;
    }

    FlashLight flashLight;
    {
        flashLight.ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
        flashLight.diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
        flashLight.specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
        flashLight.innerCutOff = 12.5f;
        flashLight.outerCutOff = 17.5f;
    }

    glEnable(GL_DEPTH_TEST);

    windowInstance->RenderLoop([&]() {
        Updater::Update();
        view.Update();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 pv = projection->GetProjection() * view.GetView();
        glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(model)));
        const glm::vec3 position = view.GetPosition();
        // mvp matrix and viewPos;
        {
            glUniformMatrix4fv(shader.GetLocation("pv"), 1, GL_FALSE, &pv[0][0]);
            glUniformMatrix4fv(shader.GetLocation("model"), 1, GL_FALSE, &model[0][0]);
            glUniform3f(shader.GetLocation("observerPos"), position.x, position.y, position.z);
            glUniformMatrix3fv(shader.GetLocation("normalMat"), 1, GL_FALSE, &normalMat[0][0]);
        }
        // material
        {
            glUniform3f(shader.GetLocation("material.ambientColor"), material.ambientColor.x, material.ambientColor.y, material.ambientColor.z);
            glUniform3f(shader.GetLocation("material.diffuseColor"), material.diffuseColor.x, material.diffuseColor.y, material.diffuseColor.z);
            glUniform3f(shader.GetLocation("material.specularColor"), material.specularColor.x, material.specularColor.y, material.specularColor.z);
            glUniform1f(shader.GetLocation("material.shininess"), material.shininess);
        }

        flashLight.position = view.GetPosition();
        flashLight.direction = view.GetDirection();
        // flashLight
        {
            glUniform3f(shader.GetLocation("flashLight.position"), flashLight.position.x, flashLight.position.y, flashLight.position.z);
            glUniform3f(shader.GetLocation("flashLight.direction"), flashLight.direction.x, flashLight.direction.y, flashLight.direction.z);
            glUniform3f(shader.GetLocation("flashLight.ambientColor"), flashLight.ambientColor.x, flashLight.ambientColor.y, flashLight.ambientColor.z);
            glUniform3f(shader.GetLocation("flashLight.diffuseColor"), flashLight.diffuseColor.x, flashLight.diffuseColor.y, flashLight.diffuseColor.z);
            glUniform3f(shader.GetLocation("flashLight.specularColor"), flashLight.specularColor.x, flashLight.specularColor.y, flashLight.specularColor.z);
            glUniform1f(shader.GetLocation("flashLight.innerCutOff"), cos(glm::radians(flashLight.innerCutOff)));
            glUniform1f(shader.GetLocation("flashLight.outerCutOff"), cos(glm::radians(flashLight.outerCutOff)));
        }
        // DirectionalLight
        {
            glUniform3f(shader.GetLocation("directionalLight.direction"), directionalLight.direction.x, directionalLight.direction.y, directionalLight.direction.z);
            glUniform3f(shader.GetLocation("directionalLight.ambientColor"), directionalLight.ambientColor.x, directionalLight.ambientColor.y, directionalLight.ambientColor.z);
            glUniform3f(shader.GetLocation("directionalLight.diffuseColor"), directionalLight.diffuseColor.x, directionalLight.diffuseColor.y, directionalLight.diffuseColor.z);
            glUniform3f(shader.GetLocation("directionalLight.specularColor"), directionalLight.specularColor.x, directionalLight.specularColor.y, directionalLight.specularColor.z);
        }
        // SpotLight
        {
            glUniform3f(shader.GetLocation("spotLight[0].position"), spotLight[0].position.x, spotLight[0].position.y, spotLight[0].position.z);
            glUniform3f(shader.GetLocation("spotLight[0].ambientColor"), spotLight[0].ambientColor.x, spotLight[0].ambientColor.y, spotLight[0].ambientColor.z);
            glUniform3f(shader.GetLocation("spotLight[0].diffuseColor"), spotLight[0].diffuseColor.x, spotLight[0].diffuseColor.y, spotLight[0].diffuseColor.z);
            glUniform3f(shader.GetLocation("spotLight[0].specularColor"), spotLight[0].specularColor.x, spotLight[0].specularColor.y, spotLight[0].specularColor.z);
            glUniform1f(shader.GetLocation("spotLight[0].constant"), spotLight[0].constant);
            glUniform1f(shader.GetLocation("spotLight[0].linear"), spotLight[0].linear);
            glUniform1f(shader.GetLocation("spotLight[0].quadratic"), spotLight[0].quadratic);

            glUniform3f(shader.GetLocation("spotLight[1].position"), spotLight[1].position.x, spotLight[1].position.y, spotLight[1].position.z);
            glUniform3f(shader.GetLocation("spotLight[1].ambientColor"), spotLight[1].ambientColor.x, spotLight[1].ambientColor.y, spotLight[1].ambientColor.z);
            glUniform3f(shader.GetLocation("spotLight[1].diffuseColor"), spotLight[1].diffuseColor.x, spotLight[1].diffuseColor.y, spotLight[1].diffuseColor.z);
            glUniform3f(shader.GetLocation("spotLight[1].specularColor"), spotLight[1].specularColor.x, spotLight[1].specularColor.y, spotLight[1].specularColor.z);
            glUniform1f(shader.GetLocation("spotLight[1].constant"), spotLight[1].constant);
            glUniform1f(shader.GetLocation("spotLight[1].linear"), spotLight[1].linear);
            glUniform1f(shader.GetLocation("spotLight[1].quadratic"), spotLight[1].quadratic);

            glUniform3f(shader.GetLocation("spotLight[2].position"), spotLight[2].position.x, spotLight[2].position.y, spotLight[2].position.z);
            glUniform3f(shader.GetLocation("spotLight[2].ambientColor"), spotLight[2].ambientColor.x, spotLight[2].ambientColor.y, spotLight[2].ambientColor.z);
            glUniform3f(shader.GetLocation("spotLight[2].diffuseColor"), spotLight[2].diffuseColor.x, spotLight[2].diffuseColor.y, spotLight[2].diffuseColor.z);
            glUniform3f(shader.GetLocation("spotLight[2].specularColor"), spotLight[2].specularColor.x, spotLight[2].specularColor.y, spotLight[2].specularColor.z);
            glUniform1f(shader.GetLocation("spotLight[2].constant"), spotLight[2].constant);
            glUniform1f(shader.GetLocation("spotLight[2].linear"), spotLight[2].linear);
            glUniform1f(shader.GetLocation("spotLight[2].quadratic"), spotLight[2].quadratic);

            glUniform3f(shader.GetLocation("spotLight[3].position"), spotLight[3].position.x, spotLight[3].position.y, spotLight[3].position.z);
            glUniform3f(shader.GetLocation("spotLight[3].ambientColor"), spotLight[3].ambientColor.x, spotLight[3].ambientColor.y, spotLight[3].ambientColor.z);
            glUniform3f(shader.GetLocation("spotLight[3].diffuseColor"), spotLight[3].diffuseColor.x, spotLight[3].diffuseColor.y, spotLight[3].diffuseColor.z);
            glUniform3f(shader.GetLocation("spotLight[3].specularColor"), spotLight[3].specularColor.x, spotLight[3].specularColor.y, spotLight[3].specularColor.z);
            glUniform1f(shader.GetLocation("spotLight[3].constant"), spotLight[3].constant);
            glUniform1f(shader.GetLocation("spotLight[3].linear"), spotLight[3].linear);
            glUniform1f(shader.GetLocation("spotLight[3].quadratic"), spotLight[3].quadratic);
        }

        glDrawArrays(GL_TRIANGLES, 0, 36);
        });

    EventManager::Terminate();
    Window::Terminate();
}

#endif