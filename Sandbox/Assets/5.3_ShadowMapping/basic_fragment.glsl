#version 460 core
in VS_OUT
{
	vec3 v_Normal;
	vec2 v_TexCoord;
	vec4 v_FragPos;
	vec4 v_FragPosInLightClipSpace;
} fs_in;

uniform sampler2D tex;

out vec4 color;

void main()
{
	color = texture(tex, fs_in.v_TexCoord);
}