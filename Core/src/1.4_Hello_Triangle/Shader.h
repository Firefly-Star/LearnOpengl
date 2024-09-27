#pragma once
#include <glad/glad.h>
#include <vector>
#include <string>

namespace Firefly
{
	class Shader
	{
	public:
		enum class Type
		{
			None = 0,
			VertexShader, FragmentShader
		};
		struct File
		{
			Type type;
			std::string fileName;
		};
	public:
		Shader(const std::vector<File>& files);
		Shader(Shader&& other) noexcept;
		Shader& operator=(Shader&& other) noexcept;
		~Shader();
		void Bind();
		unsigned int GetLocation(const char* name) { return glGetUniformLocation(m_RendererId, name); }
	private:
		unsigned int m_RendererId;

		unsigned int Compile(const File& file);

		unsigned int TypeToGLEnum(Type type);
	};
}