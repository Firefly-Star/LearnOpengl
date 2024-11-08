#version 460 core

out vec4 color;

in VS_OUT
{
	vec4 v_FragPos;
	vec3 v_Normal;
	vec2 v_TexCoord;
	vec3 v_TangentLightPos;
	vec3 v_TangentViewPos;
} fs_in;

struct Model
{
	sampler2D texture_diffuse0;
	sampler2D texture_specular0;
	sampler2D texture_normal0;
	sampler2D texture_parallax0;
};

uniform Model model;

void main()
{
	color = vec4(vec3(texture(model.texture_diffuse0, fs_in.v_TexCoord)), 1.0f);
}