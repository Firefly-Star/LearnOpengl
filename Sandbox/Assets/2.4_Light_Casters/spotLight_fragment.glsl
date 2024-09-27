#version 460 core
in vec3 v_Normal;
in vec3 v_FragPos;

out vec4 color;

uniform vec3 observerPos;

struct Light
{
	vec3 position;

	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;

	float constant;
	float linear;
	float quadratic;
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
	float len = length(light.position - v_FragPos);
	float attenuation = 1.0f / (light.constant + light.linear * len + light.quadratic * len * len);	

	vec3 ambient = material.ambientColor * light.ambientColor;

	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(light.position - v_FragPos);
	vec3 diffuse = material.diffuseColor * max(dot(lightDir, norm), 0.0f) * light.diffuseColor;

	vec3 observeDir = normalize(observerPos - v_FragPos);
	vec3 halfWay = normalize(observeDir + lightDir);
	vec3 specular = material.specularColor * pow(max(dot(halfWay, norm), 0.0f), material.shininess) * light.specularColor;

	color = vec4((ambient + diffuse + specular) * attenuation, 1.0f);
}