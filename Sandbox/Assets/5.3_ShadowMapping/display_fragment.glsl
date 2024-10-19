#version 460 core

in vec2 v_TexCoord;
uniform sampler2D tex;

out vec4 color;

void main()
{
	color = vec4((texture(tex, v_TexCoord).r) * vec3(1.0f), 1.0f);
}