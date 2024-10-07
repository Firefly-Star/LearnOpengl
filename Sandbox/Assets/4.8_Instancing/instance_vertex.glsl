#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec2 a_TexCoord;

layout(location = 7) in vec4 a_ModelMatrow1;
layout(location = 8) in vec4 a_ModelMatrow2;
layout(location = 9) in vec4 a_ModelMatrow3;
layout(location = 10) in vec4 a_ModelMatrow4;
layout(std140) uniform Mat
{
	mat4 pvMat;
};

out vec2 v_TexCoord;


void main()
{
	mat4 a_ModelMat = mat4(a_ModelMatrow1, a_ModelMatrow2, a_ModelMatrow3, a_ModelMatrow4);
	gl_Position = pvMat * a_ModelMat * vec4(a_Position, 1.0f);
	v_TexCoord = a_TexCoord;
}