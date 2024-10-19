#version 460 core
out vec4 color;

in vec3 v_Color;

void main()
{
	color = vec4(gl_FragCoord.z * vec3(1.0f, 1.0f, 1.0f), 1.0f);
}