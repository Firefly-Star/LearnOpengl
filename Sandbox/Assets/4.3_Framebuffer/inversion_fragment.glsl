#version 460 core

in vec2 v_TexCoord;

out vec4 color;

uniform sampler2D tex;

void main()
{
	vec4 texColor = texture(tex, v_TexCoord);
	color = vec4(1.0 - texColor.rgb, texColor.a);
}