#version 460 core

layout(location = 0) in vec3 a_Position;

out vec3 v_TexCoord;

uniform mat4 pvMat;

void main()
{
	gl_Position = (pvMat * vec4(a_Position, 1.0f)).xyww;
	v_TexCoord = a_Position;
}