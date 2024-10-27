#version 460 core
in vec3 v_TexCoord;

uniform samplerCube tex;

out vec4 color;

void main()
{
	float depth = texture(tex, v_TexCoord).r;
	color = vec4(depth * vec3(1.0f), 1.0f);
}