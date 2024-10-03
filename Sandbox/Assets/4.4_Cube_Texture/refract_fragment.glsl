#version 460 core

in vec3 v_Normal;
in vec3 v_FragPos;

out vec4 color;

uniform vec3 viewPos;
uniform samplerCube tex;
uniform float ratio;

void main()
{
	vec3 viewDir = normalize(v_FragPos - viewPos);
	vec3 refractDir = refract(viewDir, normalize(v_Normal), ratio);
	color = texture(tex, refractDir);
}