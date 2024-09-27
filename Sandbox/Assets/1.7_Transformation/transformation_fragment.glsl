#version 460 core
in vec3 v_color;
in vec2 v_texCoord;

out vec4 color;

uniform sampler2D tex1;

void main()
{
	color = (texture(tex1, v_texCoord)) * vec4(v_color, 1.0f);
}