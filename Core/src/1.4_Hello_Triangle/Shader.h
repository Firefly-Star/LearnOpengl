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
			VertexShader, FragmentShader, GeomotryShader
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
		void SetUniform(const char* name, int i);
		void SetUniform(const char* name, float f);
		void SetUniform(const char* name, glm::vec3 const& v3);
		void SetUniform(const char* name, glm::vec4 const& v4);
		void SetUniform(const char* name, glm::mat3 const& m3);
		void SetUniform(const char* name, glm::mat4 const& m4);
	private:
		unsigned int m_RendererId;
		std::unordered_map<const char*, int> m_UniformCache;

		unsigned int Compile(const File& file);

		unsigned int TypeToGLEnum(Type type);
		
		unsigned int GetLocation(const char* name);
	};
}