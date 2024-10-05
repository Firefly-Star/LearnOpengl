#pragma once
#include <glad/glad.h>
#include <vector>
#include <string>

#include "Macros.h"

namespace Firefly
{
	class Shader
	{
		MOVE_ONLY(Shader);
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
		~Shader();
		void Bind();
		void BindUniformBlock(const char* name, unsigned int slot = 0);
		unsigned int GetLocation(const char* name) { return glGetUniformLocation(m_RendererId, name); }
	private:
		unsigned int m_RendererId;

		unsigned int Compile(const File& file);

		unsigned int TypeToGLEnum(Type type);
	};
}