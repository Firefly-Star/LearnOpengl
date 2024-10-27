#version 460 core

in VS_OUT
{
	vec4 v_FragPos;
	vec3 v_Normal;
	vec2 v_TexCoord;
} fs_in;

out vec4 color;

struct Model
{
	sampler2D texture_diffuse0;
	sampler2D texture_specular0;
};

uniform Model model;
uniform vec3 lightPos;
uniform samplerCube depthMap;
uniform vec3 viewPos;
uniform float farPlane;

bool CalculateShadow(vec3 fragPos, vec3 lightDir, vec3 norm)
{
	float nowDist = length(fragPos - lightPos) / farPlane;
	
	bool shadow;
	if (nowDist >= 1.0f)
	{
		shadow = false;	
	}
	else
	{
		float closestDist = texture(depthMap, -lightDir).r;
		float bias = 0.005f;
		shadow = nowDist > (closestDist + bias);
	}
	return shadow;
}

vec3 CalculateLight(vec3 fragPos, vec3 lightDir, vec3 norm, vec3 texColor)
{
	float diffuseStrength = 0.4f;
	float specularStrength = 1.0f;
	float shininess = 512.0f;

	vec3 viewDir = normalize(viewPos - fragPos);

	vec3 halfWay = normalize(viewDir + lightDir);

	vec3 specularColoro = pow(texture(model.texture_specular0, fs_in.v_TexCoord).xyz, vec3(2.2f));

	vec3 diffuseColor = diffuseStrength * max(dot(norm, lightDir), 0.0f) * texColor;
	vec3 specularColor = specularStrength * pow(max(dot(norm, halfWay), 0.0f), shininess) * specularColoro;
	
	return (diffuseColor + specularColor);
}

void main()
{
	vec3 fragPos = fs_in.v_FragPos.xyz / fs_in.v_FragPos.w;
	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 norm = normalize(fs_in.v_Normal);

	bool shadow = CalculateShadow(fragPos, lightDir, norm);

	float nowDist = length(fragPos - lightPos) / farPlane;
	float closestDist = texture(depthMap, -lightDir).r;
	color = shadow? vec4(0.0f, 0.0f, 0.0f, 1.0f) : vec4(1.0f, 1.0f, 1.0f, 1.0f);
}