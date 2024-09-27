#version 460 core

in vec2 v_TexCoord;

out vec4 color;

uniform sampler2D tex;

void main()
{
	vec4 texColor = texture(tex, v_TexCoord);
	float average = (texColor.r + texColor.g + texColor.b) / 3.0f;
	color = vec4(average, average, average, texColor.a);
}