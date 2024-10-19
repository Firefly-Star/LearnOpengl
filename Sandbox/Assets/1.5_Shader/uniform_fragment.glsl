#version 460 core
out vec4 color;

uniform float c;

void main()
{
	color = vec4(0.5f, 0.6f, 0.7f, 1.0f);
	//color = vec4(c, c, c, 1.0);
}