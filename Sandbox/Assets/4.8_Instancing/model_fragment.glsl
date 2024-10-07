#version 460 core
in vec2 v_TexCoord;

out vec4 color;
struct Model
{
	sampler2D texture_diffuse0;
};

uniform Model model;

void main()
{
	color = texture(model.texture_diffuse0, v_TexCoord);
}