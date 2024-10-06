#include "Entrypoint.h"
#if CHECK_ACTIVE(4, 7, 0)

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include "Core.h"

using namespace Firefly;
#if 1
int main()
{
	Window::Init(800, 800);
	Window& window = Window::GetInstance();
	EventManager::Init();
	ShaderLibrary::Init();
	SpriteRenderer::Init();
	SkyboxRenderer::Init();

	Shader basicShader({
		{Shader::Type::VertexShader, ASSET("3.2_Model/model_vertex.glsl")},
		{Shader::Type::FragmentShader, ASSET("3.2_Model/model_fragment.glsl")}
		});
	Shader explodeShader({
		{Shader::Type::VertexShader, ASSET("4.7_Geometry_Shader/modelExplode_vertex.glsl")},
		{Shader::Type::FragmentShader, ASSET("4.7_Geometry_Shader/modelExplode_fragment.glsl")},
		{Shader::Type::GeomotryShader, ASSET("4.7_Geometry_Shader/modelExplode_geometry.glsl")}
		});
	Shader normalShader({
		{ Shader::Type::VertexShader, ASSET("4.7_Geometry_Shader/modelNormal_vertex.glsl") },
		{ Shader::Type::FragmentShader, ASSET("4.7_Geometry_Shader/modelNormal_fragment.glsl") },
		{ Shader::Type::GeomotryShader, ASSET("4.7_Geometry_Shader/modelNormal_geometry.glsl") }
	});

	Model ourModel(ASSET("3.2_Model/nanosuit/nanosuit.obj"));

	std::array<float, 4> params = { glm::radians(45.0f), 1.0f, 0.1f, 100.0f };
	Projection* projection = new PerspectiveProjection(params);
	View view;
	view.SetSpeed({ 3.0f, 3.0f, 3.0f, 0.5f });

	UniformBuffer ubo(4 * 4 * sizeof(float));
	ubo.Bind(1);
	basicShader.BindUniformBlock("Mat", 1);
	explodeShader.BindUniformBlock("Mat", 1);
	normalShader.BindUniformBlock("Mat", 1);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -5.0f, -5.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	Firefly::FlashLight light;
	{
		
		light.ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
		light.diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
		light.specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
		light.innerCutOff = cos(glm::radians(12.5f));
		light.outerCutOff = cos(glm::radians(17.5f));
		glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
		{
			basicShader.Bind();
			basicShader.SetUniform("flashLight.ambientColor", light.ambientColor);
			basicShader.SetUniform("flashLight.diffuseColor", light.diffuseColor);
			basicShader.SetUniform("flashLight.specularColor", light.specularColor);
			basicShader.SetUniform("flashLight.innerCutOff", light.innerCutOff);
			basicShader.SetUniform("flashLight.outerCutOff", light.outerCutOff);
			
			basicShader.SetUniform("modelMat", model);
			basicShader.SetUniform("normalMat", normalMat);
		}
		{
			explodeShader.Bind();
			explodeShader.SetUniform("flashLight.ambientColor", light.ambientColor);
			explodeShader.SetUniform("flashLight.diffuseColor", light.diffuseColor);
			explodeShader.SetUniform("flashLight.specularColor", light.specularColor);
			explodeShader.SetUniform("flashLight.innerCutOff", light.innerCutOff);
			explodeShader.SetUniform("flashLight.outerCutOff", light.outerCutOff);
			
			explodeShader.SetUniform("modelMat", model);
			explodeShader.SetUniform("normalMat", normalMat);
		}
		{
			normalShader.Bind();
			normalShader.SetUniform("modelMat", model);
		}
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	window.RenderLoop([&]() {
		Updater::Update();
		view.Update();
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 pv = projection->GetProjection() * view.GetView();
		ubo.FillSub(0, sizeof(glm::mat4), &pv[0][0]);

		// Basic model
		basicShader.Bind();

		basicShader.SetUniform("flashLight.position", view.GetPosition());
		basicShader.SetUniform("flashLight.direction", view.GetDirection());
		basicShader.SetUniform("observePos", view.GetPosition());
		ourModel.Render(basicShader);
		
		// Explode model
		float time = Time::GetTime();
		explodeShader.Bind();
		
		explodeShader.SetUniform("flashLight.position", view.GetPosition());
		explodeShader.SetUniform("flashLight.direction", view.GetDirection());
		explodeShader.SetUniform("observePos", view.GetPosition());
		explodeShader.SetUniform("manitude", 1.0f);
		explodeShader.SetUniform("time", time);
		ourModel.Render(explodeShader);

		// Normal model
		glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(view.GetView() * model)));
		normalShader.Bind();
		normalShader.SetUniform("normalMat", normalMat);
		normalShader.SetUniform("projectionMat", projection->GetProjection());
		normalShader.SetUniform("length", 0.05f);
		ourModel.Render(normalShader);
		});

	SkyboxRenderer::Terminate();
	SpriteRenderer::Terminate();
	ShaderLibrary::Terminate();
	EventManager::Terminate();
	Window::Terminate();
}
#endif

#endif