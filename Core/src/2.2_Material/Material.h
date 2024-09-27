#pragma once
#include <glm.hpp>
namespace Firefly
{
	struct Material
	{
		glm::vec3 ambientColor;
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;
		float shininess;
	};
}