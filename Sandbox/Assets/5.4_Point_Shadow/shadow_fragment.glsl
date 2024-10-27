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

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

float CalculateShadow(vec3 fragPos, vec3 lightDir, vec3 norm)
{
	float nowDist = length(fragPos - lightPos) / farPlane;
	float diskRadius = 0.005f;

	float shadow = 0.0f;
	if (nowDist >= 1.0f)
	{
		shadow = 1.0f;	
	}
	else
	{
		for (int i = 0;i < 20; ++i)
		{
			float closestDist = texture(depthMap, -lightDir + diskRadius * sampleOffsetDirections[i]).r;
			float bias = 0.0015f;
			shadow += nowDist > (closestDist + bias) ? 0.0f : 1.0f;
		}
		shadow /= 20.0f;
	}
	return shadow;
}

vec3 CalculateLight(vec3 fragPos, vec3 lightDir, vec3 norm, vec3 texColor)
{
	float diffuseStrength = 0.3f;
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
	float ambientStrength = 0.05f;
	vec3 texColor = pow(texture(model.texture_diffuse0, fs_in.v_TexCoord).xyz, vec3(2.2f));

	vec3 ambientColor = ambientStrength * texColor;
	vec3 fragPos = fs_in.v_FragPos.xyz / fs_in.v_FragPos.w;
	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 norm = normalize(fs_in.v_Normal);

	float shadow = CalculateShadow(fragPos, lightDir, norm);
	color = vec4(ambientColor + CalculateLight(fragPos, lightDir, norm, texColor) * shadow, 1.0f);
	//color = vec4(CalculateLight(fragPos, lightDir, norm, texColor), 1.0f);
	color = pow(color, vec4(1.0f / 2.2f));
	//color = shadow? vec4(0.0f, 0.0f, 0.0f, 1.0f) : vec4(1.0f, 1.0f, 1.0f, 1.0f);
}