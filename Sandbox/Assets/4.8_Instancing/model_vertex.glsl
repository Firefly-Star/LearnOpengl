#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec2 a_TexCoord;
layout(std140) uniform Mat
{
	mat4 pvMat;
};

uniform mat4 modelMat;

out vec2 v_TexCoord;

void main()
{
	gl_Position = pvMat * modelMat * vec4(a_Position, 1.0f);
	v_TexCoord = a_TexCoord;
}