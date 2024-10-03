#version 460 core

in vec3 v_Normal;
in vec3 v_FragPos;

out vec4 color;

uniform vec3 viewPos;
uniform samplerCube tex;

void main()
{
	vec3 viewDir = -normalize(viewPos - v_FragPos);
	vec3 reflectDir = reflect(viewDir, normalize(v_Normal));
	color = texture(tex, reflectDir);
}