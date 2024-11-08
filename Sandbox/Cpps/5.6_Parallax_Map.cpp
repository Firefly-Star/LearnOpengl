#include "Entrypoint.h"
#if CHECK_ACTIVE(5, 6, 0)

#include "Core.h"
#include "crtdbg.h"
using namespace Firefly;
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(169);
	Window::Init(1000, 1000);
	SingletonManager::Init();

	float vertices[] = {
		// Position        , Normal          , Texcoord  , Tangent         , Bitangent
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
				{VertexArray::Type::Float, 3},
			}, vbo
		}
		});
	vao.UnBind();
	Texture2D brickDiffuse(ASSET("5.6_Parallax_Map/bricks2.jpg"));
	Texture2D brickNormal(ASSET("5.6_Parallax_Map/bricks2_normal.jpg"));
	Texture2D brickDisp(ASSET("5.6_Parallax_Map/bricks2_disp.jpg"));

	Shader shader({
		{Shader::Type::VertexShader, ASSET("5.6_Parallax_Map/parallax_vertex.glsl")},
		{Shader::Type::FragmentShader, ASSET("5.6_Parallax_Map/parallax_fragment.glsl")}
		});

	PerspectiveProjection proj({ glm::radians(45.0f), 1.0f, 0.1f, 100.0f });
	View view;
	view.SetSpeed({ 4.0f, 4.0f, 4.0f, 1.0f });

	glm::mat4 e(1.0f);
	glm::mat4 modelMat = glm::rotate(e, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(e, glm::vec3(3.0f, 1.0f, 3.0f));

	Window::GetInstance().RenderLoop([&]() {
		Updater::Update();
		view.Update();
		glClearColor(0.2f, 0.3f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Bind();
		vao.Bind();
		brickDiffuse.Bind(1);
		shader.SetUniform("pvMat", proj.GetProjection() * view.GetView());
		shader.SetUniform("modelMat", modelMat);
		shader.SetUniform("model.texture_diffuse0", 1);

		glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);
		});
	SingletonManager::Terminate();
	Window::Terminate();
}

#endif