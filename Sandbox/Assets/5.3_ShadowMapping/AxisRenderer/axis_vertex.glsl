#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;

out vec3 v_Color;

uniform mat4 pvmMat;

void main()
{
	v_Color = a_Color;
	vec4 mid = pvmMat * vec4(a_Position, 1.0f);
	//gl_Position = vec4(mid.xy, -mid.w, mid.w);
	gl_Position = mid;
}