#pragma once
#include <glad/glad.h>
#include <vector>
#include <string>
#include <unordered_map>

#include "glm.hpp"
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
			VertexShader, FragmentShader, GeometryShader
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
		static void UnBind();
		void BindUniformBlock(const char* name, unsigned int slot = 0);
		void SetUniform(std::string const& name, int i);
		void SetUniform(std::string const& name, float f);
		void SetUniform(std::string const& namr, glm::vec2 const& v2, unsigned int count = 1);
		void SetUniform(std::string const& name, glm::vec3 const& v3, unsigned int count = 1);
		void SetUniform(std::string const& name, glm::vec4 const& v4, unsigned int count = 1);
		void SetUniform(std::string const& name, glm::mat3 const& m3, unsigned int count = 1);
		void SetUniform(std::string const& name, glm::mat4 const& m4, unsigned int count = 1);
	private:
		unsigned int m_RendererId;
		std::unordered_map<std::string, int> m_UniformCache;

		unsigned int Compile(const File& file);

		unsigned int TypeToGLEnum(Type type);
		
		unsigned int GetLocation(std::string const& name);
	};
}