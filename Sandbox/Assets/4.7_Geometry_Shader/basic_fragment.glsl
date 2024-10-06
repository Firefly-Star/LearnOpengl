#version 460 core

out vec4 color;

in vec3 g_Color;

void main()
{
	color = vec4(g_Color, 1.0f);
}