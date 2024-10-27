#version 460 core

in GS_OUT
{
	vec4 v_FragPos;
} fs_in;

uniform vec3 lightPos;
uniform float farPlane;

void main()
{
	vec3 fragPos = (fs_in.v_FragPos.xyz) / (fs_in.v_FragPos.w); 
	float depth = length(lightPos - fragPos) / farPlane;
	gl_FragDepth = depth;
}