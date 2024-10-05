#version 460 core

layout(location = 0) in vec3 a_Position;

uniform mat4 pvMat;

out vec3 v_TexCoord;

void main()
{
	gl_Position = (pvMat * vec4(a_Position, 1.0f)).xyww;
	v_TexCoord = a_Position;
}