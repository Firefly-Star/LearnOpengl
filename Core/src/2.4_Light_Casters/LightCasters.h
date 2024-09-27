#pragma once
#include <glm.hpp>


namespace Firefly
{
	struct DirectionalLight
	{
		glm::vec3 direction;

		glm::vec3 ambientColor;
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;
	};

	struct SpotLight
	{
		glm::vec3 position;

		glm::vec3 ambientColor;
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;

		float constant;
		float linear;
		float quadratic;
	};

	struct FlashLight
	{
		glm::vec3 position;
		glm::vec3 direction;

		glm::vec3 ambientColor;
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;

		float innerCutOff;
		float outerCutOff;
	};
}