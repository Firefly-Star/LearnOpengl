#version 460 core
in vec2 v_TexCoord;

out vec4 color;

uniform mat3 kernel;
uniform float offset;
uniform sampler2D tex;

void main()
{
	vec2 offsetMat[9] = vec2[9](
		vec2(-offset, offset),
		vec2(0, offset),
		vec2(offset, offset),
		vec2(-offset, 0),
		vec2(0, 0),
		vec2(offset, 0),
		vec2(-offset, -offset),
		vec2(0, -offset),
		vec2(offset, -offset)
	); 
	vec3 result = vec3(0.0f, 0.0f, 0.0f);
	for (int i = 0;i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			result += texture(tex, v_TexCoord + offsetMat[i * 3 + j]).rgb * kernel[i][j];
		}
	}
	color = vec4(result, texture(tex, v_TexCoord).a);
}