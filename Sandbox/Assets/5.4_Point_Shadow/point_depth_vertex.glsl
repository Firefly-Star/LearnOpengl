#version 460 core
layout(location = 0) in vec3 a_Position;

uniform mat4 modelMat;

void main()
{
	gl_Position = modelMat * vec4(a_Position, 1.0f);
}