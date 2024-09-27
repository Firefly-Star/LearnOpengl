#version 460 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_color;
layout(location = 2) in vec2 a_texCoord;

out vec2 v_texCoord;
out vec3 v_color;

void main()
{
	gl_Position = vec4(a_position, 1.0);
	v_texCoord = a_texCoord;
	v_color = a_color;
}