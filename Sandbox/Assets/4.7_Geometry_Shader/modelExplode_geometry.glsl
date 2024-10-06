#version 460 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform float manitude;
uniform float time;

in VS_OUT
{
	vec3 v_Normal;
	vec3 v_FragPos;
	vec2 v_TexCoord;
} gs_in[];

out GS_OUT
{
	vec3 v_Normal;
	vec3 v_FragPos;
	vec2 v_TexCoord;
} gs_out;

vec3 GetNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a, b));
}


vec4 Explode(in vec4 position, in vec3 normal)
{
	return position + vec4(manitude * (sin(time) + 1.0f) / 2.0f * normal, 0.0f);
}

void main()
{

	vec3 normal = GetNormal();
	for (int i = 0;i < 3; ++i)
	{
		gl_Position = Explode(gl_in[i].gl_Position, normal);
		gs_out.v_Normal = gs_in[i].v_Normal;
		gs_out.v_FragPos = gs_in[i].v_FragPos;
		gs_out.v_TexCoord = gs_in[i].v_TexCoord;
		EmitVertex();
	}

	EndPrimitive();
}