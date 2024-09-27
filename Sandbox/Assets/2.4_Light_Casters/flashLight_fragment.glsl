#version 460 core
in vec3 v_Normal;
in vec3 v_FragPos;

out vec4 color;

uniform vec3 observerPos;

struct Light
{
	vec3 position;
	vec3 direction;

	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;

	float innerCutOff;
	float outerCutOff;
};
uniform Light light;

struct Material
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	float shininess;
};
uniform Material material;


void main()
{
	vec3 norm = normalize(v_Normal);
	vec3 ambient = material.ambientColor * light.ambientColor;

	vec3 lightDir = normalize(light.position - v_FragPos);
	float angle = dot(lightDir, -light.direction);

	if (angle <= light.outerCutOff)
	{
		color = vec4(ambient, 1.0f);
	}
	else
	{
		vec3 diffuse = material.diffuseColor * max(dot(lightDir, norm), 0.0f) * light.diffuseColor;
		
		vec3 observeDir = normalize(observerPos - v_FragPos);
		vec3 halfWay = normalize(observeDir + lightDir);
		vec3 specular = material.specularColor * pow(max(dot(halfWay, norm), 0.0f), material.shininess) * light.specularColor;
		
		if (angle > light.innerCutOff)
		{
			color = vec4(ambient + diffuse + specular, 1.0f);
		}
		else
		{
			float intensity = smoothstep(light.outerCutOff, light.innerCutOff, angle);
			color = vec4(ambient + (diffuse + specular) * intensity, 1.0f);
		}
	}

}