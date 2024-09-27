#version 460 core
in vec3 v_color;
in vec2 v_texCoord;

out vec4 color;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
	color = 0.5 * texture(tex1, v_texCoord) +  0.5 * texture(tex2, v_texCoord);
}