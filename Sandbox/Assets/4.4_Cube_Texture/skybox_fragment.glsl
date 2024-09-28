#version 460 core

in vec3 v_TexCoord;

out vec4 color;

uniform samplerCube tex;

void main()
{
	color = vec4(texture(tex, v_TexCoord).rgb, 1.0f);
}