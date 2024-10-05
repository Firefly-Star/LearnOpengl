#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(std140) uniform Mat
{
	mat4 pvMat;
};

out vec3 v_Normal;
out vec3 v_FragPos;

uniform mat4 modelMat;
uniform mat3 normalMat;

void main()
{
	gl_Position = pvMat * modelMat * vec4(a_Position, 1.0f);
	v_Normal = normalMat * a_Normal;
	v_FragPos = vec3(modelMat * vec4(a_Position, 1.0f));
}
