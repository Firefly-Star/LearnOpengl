#include <iostream>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <gtc/type_ptr.hpp>
#include "Shader.h"

#include "4.8_Instancing/BindManager.h"

namespace Firefly
{
	Shader::Shader(const std::vector<File>& files)
	{
		m_RendererId = glCreateProgram();

		std::vector<unsigned int> shaders;

		for (auto file : files)
		{
			unsigned int handle = Compile(file);
			if (handle != 0)
			{
				shaders.emplace_back(handle);
			}
			else
			{
				return;
			}
		}

		for (auto shader : shaders)
		{
			glAttachShader(m_RendererId, shader);
			glDeleteShader(shader);	
		}
		glLinkProgram(m_RendererId);

		int success;
		glGetProgramiv(m_RendererId, GL_LINK_STATUS, &success);
		if (!success)
		{
			int length;
			glGetProgramiv(m_RendererId, GL_INFO_LOG_LENGTH, &length);
			char* infoLog = new char[length];
			glGetProgramInfoLog(m_RendererId, length, nullptr, infoLog);

			std::cerr << "Failed to link the program: \n" << infoLog;

			glDeleteProgram(m_RendererId);

			delete[] infoLog;
			return;
		}
	}

	Shader::Shader(Shader&& other) noexcept
		:m_RendererId(other.m_RendererId)
	{
		other.m_RendererId = 0;
	}
	
	Shader& Shader::operator=(Shader&& other) noexcept
	{
		if (this != &other)
		{
			glDeleteProgram(m_RendererId);
			m_RendererId = other.m_RendererId;
			other.m_RendererId = 0;
		}
		return *this;
	}
	
	Shader::~Shader()
	{
		glDeleteProgram(m_RendererId);
	}
	
	void Shader::Bind()
	{
		if (!BindManager::GetInstance().CheckBind(this))
			glUseProgram(m_RendererId);
	}

	void Shader::UnBind()
	{
		glUseProgram(0);
		BindManager::GetInstance().UnBind<Shader>();
	}

	void Shader::BindUniformBlock(const char* name, unsigned int slot)
	{
		unsigned int index = glGetUniformBlockIndex(m_RendererId, name);
		glUniformBlockBinding(m_RendererId, index, slot);
	}

	unsigned int Shader::Compile(const File& file)
	{
		std::ifstream fs(file.fileName);
		if (!fs.is_open())
		{
			std::cerr << "Failed to open file: " << file.fileName << "\n";
			return 0;
		}

		std::stringstream ss;
		ss << fs.rdbuf();
		std::string mid = ss.str();
		const char* content = mid.c_str();

		unsigned int shaderHandle = glCreateShader(TypeToGLEnum(file.type));
		glShaderSource(shaderHandle, 1, &(content), nullptr);
		glCompileShader(shaderHandle);

		int success;
		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			int length;
			glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length);
			char* infoLog = new char[length];
			glGetShaderInfoLog(shaderHandle, length, nullptr, infoLog);

			std::cerr << "Failed to compile the shader: " << file.fileName << "\n" << infoLog;

			glDeleteShader(shaderHandle);

			delete[] infoLog;
			return 0;
		}
		return shaderHandle;
	}

	unsigned int Shader::TypeToGLEnum(Type type)
	{
		switch (type)
		{
		case Type::VertexShader: return GL_VERTEX_SHADER;
		case Type::FragmentShader: return GL_FRAGMENT_SHADER;
		case Type::GeomotryShader: return GL_GEOMETRY_SHADER;
		}
		return 0;
	}

	void Shader::SetUniform(std::string const& name, int i)
	{
		int location = GetLocation(name);
		if (location != -1) glUniform1i(location, i);
	}

	void Shader::SetUniform(std::string const& name, float f)
	{
		int location = GetLocation(name);
		if (location != -1) glUniform1f(location, f);
	}

	void Shader::SetUniform(std::string const& name, glm::vec2 const& v2, unsigned int count)
	{
		int location = GetLocation(name);
		if (location != -1) glUniform2fv(location, count, glm::value_ptr(v2));
	}

	void Shader::SetUniform(std::string const& name, glm::vec3 const& v3, unsigned int count)
	{
		int location = GetLocation(name);
		if (location != -1) glUniform3fv(location, count, &v3[0]);
	}

	void Shader::SetUniform(std::string const& name, glm::vec4 const& v4, unsigned int count)
	{
		int location = GetLocation(name);
		if (location != -1) glUniform4fv(location, count, &v4[0]);
	}

	void Shader::SetUniform(std::string const& name, glm::mat3 const& m3, unsigned int count)
	{
		int location = GetLocation(name);
		if (location != -1) glUniformMatrix3fv(location, count, GL_FALSE, &m3[0][0]);
	}

	void Shader::SetUniform(std::string const& name, glm::mat4 const& m4, unsigned int count)
	{
		int location = GetLocation(name);
		if (location != -1) glUniformMatrix4fv(location, count, GL_FALSE, &m4[0][0]);
	}

	unsigned int Shader::GetLocation(std::string const& name)
	{
		auto it = m_UniformCache.find(name);
		if (it != m_UniformCache.end())
		{
			return it->second;
		}
		else
		{
			int location = glGetUniformLocation(m_RendererId, name.c_str());
			m_UniformCache[name] = location;
			return location;
		}
	}
}
