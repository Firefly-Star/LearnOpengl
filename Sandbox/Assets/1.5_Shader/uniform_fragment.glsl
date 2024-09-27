#version 330 core
out vec4 color;

uniform float c;

void main()
{
	color = vec4(c, c, c, 1.0);
}