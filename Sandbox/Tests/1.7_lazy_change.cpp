#include "Entrypoint.h"
#if CHECK_ACTIVE(1, 7, 1)
	
#include <iostream>
#include <glad/glad.h>
#include "Core.h"

class VertexArray2
{
public:
	VertexArray2()
	{
		glGenVertexArrays(1, &m_RendererId);
	}
	~VertexArray2()
	{
		glDeleteVertexArrays(1, &m_RendererId);
	}
	void Bind()
	{
		glBindVertexArray(m_RendererId);
	}
	static void UnBind()
	{
		glBindVertexArray(0);
	}
private:
	unsigned int m_RendererId;
};
using namespace Firefly;

int main()
{
	Window::Init();
	BindManager::Init();
	VertexArray vao1;
	VertexArray vao2;
	VertexArray2 vao3;
	VertexArray2 vao4;

	unsigned int count = 10000000;
	
	std::cout << "\n";
	Benchmark test1("Single vao with lazy change");
	VertexArray::UnBind();
	test1.Begin();
	for (unsigned int i = 0; i < count; ++i)
	{
		vao1.Bind();
	}
	test1.End();
	test1.Print();
		

	Benchmark test2("Single vao without lazy change");
	VertexArray::UnBind();
	test2.Begin();
	for (unsigned int i = 0; i < count; ++i)
	{
		vao3.Bind();
	}
	test2.End();
	test2.Print();

	Benchmark test3("Two vao with lazy change");
	VertexArray::UnBind();
	test3.Begin();
	for (unsigned int i = 0; i < count; ++i)
	{
		vao1.Bind();
		vao2.Bind();
	}
	test3.End();
	test3.Print();

	Benchmark test4("Two vao without lazy change");
	VertexArray::UnBind();
	test4.Begin();
	for (unsigned int i = 0; i < count; ++i)
	{
		vao3.Bind();
		vao4.Bind();
	}
	test4.End();
	test4.Print();
	
}

#endif