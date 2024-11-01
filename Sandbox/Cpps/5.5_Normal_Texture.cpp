#include "Entrypoint.h"
#if CHECK_ACTIVE(5, 5, 0)

#include "Core.h"
#include "crtdbg.h"
using namespace Firefly;

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Window::Init(1000, 1000);
	SingletonManager::Init();

	Shader basicShader({
		{Shader::Type::VertexShader, ASSET("5.5_Normal_Texture/normal_vertex.glsl")},
		{Shader::Type::FragmentShader, ASSET("5.5_Normal_Texture/normal_fragment.glsl")}
		});

	unsigned char pureWhite[] = {
		255, 255, 255, 255
	};
	Texture2D brickWallDiffuse(ASSET("5.5_Normal_Texture/brickwall.jpg"));
	Texture2D brickWallNormal(ASSET("5.5_Normal_Texture/brickwall_normal.jpg"));
	Texture2D brickWallSpecular(pureWhite, 1, 1);

	float vertices[] = {
		-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	VertexArray vao;
	VertexBuffer vbo(vertices, sizeof(vertices));
	IndexBuffer ibo(indices, sizeof(indices));
	vao.Bind();
	ibo.Bind();
	vao.SetLayout({
		{
			{
				{VertexArray::Type::Float, 3},
				{VertexArray::Type::Float, 3},
				{VertexArray::Type::Float, 2},
				{VertexArray::Type::Float, 3},
				{VertexArray::Type::Float, 3}
			}, vbo
		}
		});

	PerspectiveProjection proj({ glm::radians(45.0f), 1.0f, 0.1f, 100.0f });
	View view;
	view.SetSpeed({3.0f, 3.0f, 3.0f, 1.0f});

	glm::mat4 e(1.0f);
	glm::mat4 modelMat = glm::rotate(e, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	FlashLight light;
	light.ambientColor = glm::vec3(0.1f, 0.1f, 0.1f);
	light.diffuseColor = glm::vec3(0.4f, 0.4f, 0.4f);
	light.specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	light.innerCutOff = glm::radians(10.0f);
	light.outerCutOff = glm::radians(15.0f);

	Window::GetInstance().RenderLoop([&] {
			Updater::Update();
			view.Update();
			
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			basicShader.Bind();
			brickWallDiffuse.Bind(3);
			brickWallSpecular.Bind(4);
			brickWallNormal.Bind(5);
			basicShader.SetUniform("pvMat", proj.GetProjection() * view.GetView());
			basicShader.SetUniform("modelMat", modelMat);
			basicShader.SetUniform("normalMat", glm::inverse(glm::transpose(glm::mat3(modelMat))));
			basicShader.SetUniform("model.texture_diffuse0", 3);
			basicShader.SetUniform("model.texture_specular0", 4);
			basicShader.SetUniform("model.texture_normal0", 5);
			basicShader.SetUniform("light.ambientColor", light.ambientColor);
			basicShader.SetUniform("light.diffuseColor", light.diffuseColor);
			basicShader.SetUniform("light.specularColor", light.specularColor);
			basicShader.SetUniform("light.Position", view.GetPosition());
			basicShader.SetUniform("viewPos", view.GetPosition());
			vao.Bind();
			glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);
		});
}

#endif