#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

uniform mat4 pvMat;
uniform mat4 modelMat;
uniform mat3 normalMat;

out VS_OUT
{
	vec4 v_FragPos;
	vec2 v_TexCoord;
	mat3 v_TBN;
} vs_out;


void main()
{
	gl_Position = pvMat * modelMat * vec4(a_Position, 1.0f);
	vs_out.v_FragPos = modelMat * vec4(a_Position, 1.0f);
	vs_out.v_TexCoord = a_TexCoord;
	vs_out.v_TBN = transpose(normalMat * mat3(a_Tangent, a_Bitangent, a_Normal));
}