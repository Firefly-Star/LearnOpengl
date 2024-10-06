#version 460 core
layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(std140) uniform Mat
{
	mat4 pvMat;
};

uniform mat4 modelMat;

uniform mat3 normalMat; // glm::mat3(glm::transpose(glm::inverse(viewMat * modelMat)))

out VS_OUT
{
	vec3 v_Normal;
} vs_out;

void main()
{
	gl_Position = pvMat * modelMat * vec4(a_Pos, 1.0f); // pos coordinate in clip space
	vs_out.v_Normal = normalize(normalMat * a_Normal);  // normal coordinate in clip space
}