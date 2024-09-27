#pragma once

#include <memory>
#include <mutex>
#include <unordered_map>
#include <string>
#include "Macros.h"
#include "1.4_Hello_Triangle/Shader.h"

namespace Firefly
{
	class ShaderLibrary
	{
		SINGLETON(ShaderLibrary);
	public:
		void Add(const std::string& name, Shader&& shader);
		Shader& Get(const std::string& name);
	private:
		std::unordered_map<std::string, Shader> m_Shaders;
		ShaderLibrary();
		~ShaderLibrary();
	};
}