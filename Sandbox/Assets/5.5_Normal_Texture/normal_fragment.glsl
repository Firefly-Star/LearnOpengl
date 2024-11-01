#version 460 core

in VS_OUT
{
	vec4 v_FragPos;
	vec2 v_TexCoord;
	mat3 v_TBN;
}fs_in;

out vec4 color;

struct Model
{
	sampler2D texture_diffuse0;
	sampler2D texture_specular0;
	sampler2D texture_normal0;
};

struct FlashLight
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	vec3 innerCutOff;
	vec3 outerCutOff;
	vec3 Position;
	vec3 direction;
};

uniform Model model;
uniform FlashLight light;
uniform vec3 viewPos;

void main()
{
	vec3 diffuseTex = pow(texture(model.texture_diffuse0, fs_in.v_TexCoord).xyz, vec3(2.2f));
	vec3 specularTex = pow(texture(model.texture_specular0, fs_in.v_TexCoord).xyz, vec3(2.2f));
	vec3 normalTex = texture(model.texture_normal0, fs_in.v_TexCoord).xyz;

	vec3 fragPos = fs_in.v_FragPos.xyz / fs_in.v_FragPos.w;
	vec3 norm = normalize(normalTex * 2.0f - 1.0f);

	vec3 lightDir = light.Position - fragPos;
	lightDir = normalize(fs_in.v_TBN * lightDir);
	vec3 viewDir = viewPos - fragPos;
	viewDir = normalize(fs_in.v_TBN * viewDir);
	
	vec3 halfWay = normalize(lightDir + viewDir);

	vec3 ambientColor = light.ambientColor * diffuseTex;
	vec3 diffuseColor = max(0.0f, dot(norm, lightDir)) * light.diffuseColor * diffuseTex;
	vec3 specularColor = pow(max(0.0f, dot(norm, halfWay)), 128.0f) * light.specularColor * specularTex;

	color = vec4(ambientColor + diffuseColor + specularColor, 1.0f);
	color = pow(color, vec4(1.0f / 2.2f));
}