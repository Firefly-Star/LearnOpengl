#pragma once
#include <glm.hpp>

namespace Firefly
{
	struct Light
	{
		glm::vec3 position;

		glm::vec3 ambientColor;
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;

	};
}