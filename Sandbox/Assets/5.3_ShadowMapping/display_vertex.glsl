#version 460 core
layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

uniform mat4 model;

void main()
{
	gl_Position = model * vec4(a_Position, 0.0f, 1.0f);
	v_TexCoord = a_TexCoord;
}