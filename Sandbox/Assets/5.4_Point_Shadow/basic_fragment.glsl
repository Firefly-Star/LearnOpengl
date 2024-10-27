#version 460 core

in VS_OUT
{
	vec4 v_FragPos;
	vec2 v_TexCoord;
} fs_in;

struct Model
{
	sampler2D texture_diffuse0;
};

uniform Model model;

out vec4 color;

void main()
{
	color = texture(model.texture_diffuse0, fs_in.v_TexCoord);
}