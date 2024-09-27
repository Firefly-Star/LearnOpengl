#version 460 core

in vec2 v_TexCoord;

out vec4 color;

uniform sampler2D tex;

void main()
{
	color = texture(tex, v_TexCoord);
}