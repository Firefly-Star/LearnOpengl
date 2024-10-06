#version 460 core
layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(std140) uniform Mat
{
	mat4 pvMat;
};

uniform mat4 modelMat;
uniform mat3 normalMat;

out VS_OUT
{
	vec3 v_Normal;
	vec3 v_FragPos;
	vec2 v_TexCoord;
} vs_out;

void main()
{
	gl_Position = pvMat * modelMat * vec4(a_Pos, 1.0f);
	vs_out.v_Normal = normalMat * a_Normal;
	vs_out.v_FragPos = vec3(modelMat * vec4(a_Pos, 1.0f));
	vs_out.v_TexCoord = a_TexCoord;
}