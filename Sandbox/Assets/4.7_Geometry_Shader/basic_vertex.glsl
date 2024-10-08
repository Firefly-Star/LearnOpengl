#version 460 core
layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec3 a_Color;

out VS_OUT
{
	vec3 v_Color;
} vs_out;

void main()
{
	gl_Position = vec4(a_Position, 0.0f, 1.0f);
	gl_PointSize = 10.0f;
	vs_out.v_Color = a_Color;
}