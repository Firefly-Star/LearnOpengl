#version 460 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord;

out vec2 v_texCoord;

uniform mat4 transformation;

void main()
{
	gl_Position = transformation * vec4(a_position, 1.0f);
	v_texCoord = a_texCoord;
}