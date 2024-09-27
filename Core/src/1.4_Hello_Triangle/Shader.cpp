#include <iostream>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include "Shader.h"

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
		glUseProgram(m_RendererId);
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
		}
		return 0;
	}
	
}
