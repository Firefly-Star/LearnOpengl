#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

uniform mat4 pvMat[6];

out GS_OUT
{
	vec4 v_FragPos;
} gs_out;

void main()
{
	for (int i = 0;i < 6; ++i)
	{
		gl_Layer = i;
		for (int j = 0;j < 3; ++j)
		{
			gs_out.v_FragPos = gl_in[j].gl_Position;
			gl_Position = pvMat[i] * gs_out.v_FragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}