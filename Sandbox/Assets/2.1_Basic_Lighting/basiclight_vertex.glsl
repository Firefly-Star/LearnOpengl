#version 460 core
layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;

uniform mat4 pv;
uniform mat4 model;
uniform mat3 normalMat;

out vec3 v_Normal;
out vec3 v_FragPos;

void main()
{
	gl_Position = pv * model * vec4(a_Pos, 1.0f);
	v_Normal = normalMat * a_Normal;
	v_FragPos = vec3(model * vec4(a_Pos, 1.0f));
}