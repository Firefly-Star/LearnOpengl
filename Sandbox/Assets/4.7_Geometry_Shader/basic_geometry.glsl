#version 460 core
layout(points) in;
layout(triangle_strip, max_vertices = 5) out;

in VS_OUT
{
	vec3 v_Color;
} gs_in[];

out vec3 g_Color;

void main()
{
	g_Color = gs_in[0].v_Color + vec3(-0.1f, -0.1f, 0.0f);
	gl_Position = gl_in[0].gl_Position + vec4(-0.1f, -0.1f, 0.0f, 0.0f);
	EmitVertex();
	g_Color = gs_in[0].v_Color + vec3(0.1f, -0.1f, 0.0f);
	gl_Position = gl_in[0].gl_Position + vec4(0.1f, -0.1f, 0.0f, 0.0f);
	EmitVertex();
	g_Color = gs_in[0].v_Color + vec3(-0.1f, 0.1f, 0.0f);
	gl_Position = gl_in[0].gl_Position + vec4(-0.1f, 0.1f, 0.0f, 0.0f);
	EmitVertex();
	g_Color = gs_in[0].v_Color + vec3(0.1f, 0.1f, 0.0f);
	gl_Position = gl_in[0].gl_Position + vec4(0.1f, 0.1f, 0.0f, 0.0f);
	EmitVertex();
	g_Color = gs_in[0].v_Color + vec3(0.0f, 0.2f, 0.0f);
	gl_Position = gl_in[0].gl_Position + vec4(0.0f, 0.2f, 0.0f, 0.0f);
	EmitVertex();

	EndPrimitive();
}