#version 460 core
layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in VS_OUT
{
	vec3 v_Normal; // normal vector in view space
} gs_in[];

uniform mat4 projectionMat;
uniform float length;

void main()
{
	for (int i = 0;i < 3; ++i)
	{
		gl_Position = gl_in[i].gl_Position; 
		EmitVertex();
		gl_Position = gl_in[i].gl_Position + projectionMat * vec4(length * gs_in[i].v_Normal, 0.0f);
		EmitVertex();
		EndPrimitive();
	}
}