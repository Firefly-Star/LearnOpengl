#version 460 core
in vec3 v_Normal;
in vec3 v_FragPos;

out vec4 color;

uniform vec3 observerPos;

struct Material
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	float shininess;
};
uniform Material material;

struct SpotLight
{
	vec3 position;

	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;

	float constant;
	float linear;
	float quadratic;
};
uniform SpotLight spotLight[4];

struct DirectionalLight
{
	vec3 direction;

	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
};
uniform DirectionalLight directionalLight;

struct FlashLight
{
	vec3 position;
	vec3 direction;

	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;

	float innerCutOff;
	float outerCutOff;
};
uniform FlashLight flashLight;

vec3 CalSpotLight(SpotLight light, vec3 norm, vec3 observeDir)
{
	float len = length(light.position - v_FragPos);
	float attenuation = 1.0f / (light.constant + light.linear * len + light.quadratic * len * len);	

	vec3 ambient = material.ambientColor * light.ambientColor;

	vec3 lightDir = normalize(light.position - v_FragPos);
	vec3 diffuse = material.diffuseColor * max(dot(lightDir, norm), 0.0f) * light.diffuseColor;

	vec3 halfWay = normalize(observeDir + lightDir);
	vec3 specular = material.specularColor * pow(max(dot(halfWay, norm), 0.0f), material.shininess) * light.specularColor;

	return (ambient + diffuse + specular) * attenuation;
}

vec3 CalDirectionalLight(DirectionalLight light, vec3 norm, vec3 observeDir)
{
	vec3 ambient = material.ambientColor * light.ambientColor;

	vec3 diffuse = material.diffuseColor * max(dot(-light.direction, norm), 0.0f) * light.diffuseColor;

	vec3 halfWay = normalize(observeDir - light.direction);
	vec3 specular = material.specularColor * pow(max(dot(halfWay, norm), 0.0f), material.shininess) * light.specularColor;

	return (ambient + diffuse + specular);
}

vec3 CalFlashLight(FlashLight light, vec3 norm, vec3 observeDir)
{
	vec3 ambient = material.ambientColor * light.ambientColor;

	vec3 lightDir = normalize(light.position - v_FragPos);
	float angle = dot(lightDir, -light.direction);

	if (angle <= light.outerCutOff)
	{
		return ambient;
	}
	else
	{
		vec3 diffuse = material.diffuseColor * max(dot(lightDir, norm), 0.0f) * light.diffuseColor;
		
		
		vec3 halfWay = normalize(observeDir + lightDir);
		vec3 specular = material.specularColor * pow(max(dot(halfWay, norm), 0.0f), material.shininess) * light.specularColor;
		
		if (angle > light.innerCutOff)
		{
			return ambient + diffuse + specular;
		}
		else
		{
			float intensity = smoothstep(light.outerCutOff, light.innerCutOff, angle);
			return ambient + (diffuse + specular) * intensity;
		}
	}
}

void main()
{
	vec3 result = vec3(0.0f, 0.0f, 0.0f);
	vec3 norm = normalize(v_Normal);
	vec3 observeDir = normalize(observerPos - v_FragPos);
	for (int i = 0;i < 4; ++i)
	{
		result += CalSpotLight(spotLight[i], norm, observeDir);
	}
	result += CalDirectionalLight(directionalLight, norm, observeDir);
	result += CalFlashLight(flashLight, norm, observeDir);
	color = vec4(result, 1.0f);
}