#version 460 core
in vec3 v_Normal;
in vec3 v_FragPos;

out vec4 color;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 observerPos;


void main()
{
	vec3 objectColor = vec3(1.0f, 1.0f, 1.0f);

	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

	float diffuseStrength = 0.5f;
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(lightPos - v_FragPos);
	vec3 diffuse = diffuseStrength * max(dot(lightDir, norm), 0.0f) * lightColor;

	float specularStrength = 0.5f;
	vec3 observeDir = normalize(observerPos - v_FragPos);
	vec3 halfWay = normalize(observeDir + lightDir);
	vec3 specular = specularStrength * pow(max(dot(halfWay, norm), 0.0f), 512) * lightColor;

	color = vec4((ambient + diffuse + specular) * objectColor, 1.0f);
}