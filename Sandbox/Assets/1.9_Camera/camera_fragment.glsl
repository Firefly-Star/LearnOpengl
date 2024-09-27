#version 460 core
in vec2 v_texCoord;

out vec4 color;

uniform sampler2D tex1;

void main()
{
	color = texture(tex1, v_texCoord);
}