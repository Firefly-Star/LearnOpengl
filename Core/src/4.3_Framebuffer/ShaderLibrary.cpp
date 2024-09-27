#include "ShaderLibrary.h"

namespace Firefly
{
	std::unique_ptr<ShaderLibrary> ShaderLibrary::s_Instance = nullptr;
	std::once_flag ShaderLibrary::s_OnceFlag;
	
	void ShaderLibrary::Add(const std::string& name, Shader&& shader)
	{
		if (m_Shaders.find(name) == m_Shaders.end())
		{
			m_Shaders.emplace(name, std::move(shader));
		}
	}
	
	Shader& ShaderLibrary::Get(const std::string& name)
	{
		auto it = m_Shaders.find(name);
		if (it != m_Shaders.end())
		{
			return it->second;
		}
		throw std::runtime_error("Failed to find shader: " + name);
	}
	
	ShaderLibrary::ShaderLibrary()
	{
		m_Shaders.emplace("BasicTexture", Shader({
			{Shader::Type::VertexShader, ASSET("4.3_Framebuffer/pure_vertex.glsl")},
			{Shader::Type::FragmentShader, ASSET("4.3_Framebuffer/pure_fragment.glsl")},
			}));
	}

	ShaderLibrary::~ShaderLibrary()
	{
	}
}