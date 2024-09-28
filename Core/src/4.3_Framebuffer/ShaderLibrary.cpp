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
			return;
		}
		throw std::runtime_error("Shader name repeated: " + name);
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
		m_Shaders.emplace("BasicTexture2D", Shader({
			{Shader::Type::VertexShader, ASSET("4.3_Framebuffer/pure_vertex.glsl")},
			{Shader::Type::FragmentShader, ASSET("4.3_Framebuffer/pure_fragment.glsl")},
			}));
		m_Shaders.emplace("InversionTexture2D", Shader({
			{Shader::Type::VertexShader, ASSET("4.3_Framebuffer/pure_vertex.glsl")},
			{Shader::Type::FragmentShader, ASSET("4.3_Framebuffer/inversion_fragment.glsl")},
			}));
		m_Shaders.emplace("GrayScaleTexture2D", Shader({
			{Shader::Type::VertexShader, ASSET("4.3_Framebuffer/pure_vertex.glsl")},
			{Shader::Type::FragmentShader, ASSET("4.3_Framebuffer/grayscale_fragment.glsl")},
			}));
		m_Shaders.emplace("KernelTexture2D", Shader({
			{Shader::Type::VertexShader, ASSET("4.3_Framebuffer/pure_vertex.glsl")},
			{Shader::Type::FragmentShader, ASSET("4.3_Framebuffer/kernel_fragment.glsl")},
			}));
		m_Shaders.emplace("Skybox3D", Shader({
			{Shader::Type::VertexShader, ASSET("4.4_Cube_Texture/skybox_vertex.glsl")},
			{Shader::Type::FragmentShader, ASSET("4.4_Cube_Texture/skybox_fragment.glsl")}
			}));
	}

	ShaderLibrary::~ShaderLibrary()
	{
	}
}