#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

uniform mat4 pvMat;
uniform mat4 modelMat;
uniform mat3 normalMat;
uniform vec3 lightPos;
uniform vec3 viewPos;

out VS_OUT
{
	vec4 v_FragPos;
	vec3 v_Normal;
	vec2 v_TexCoord;
	vec3 v_TangentLightPos;
	vec3 v_TangentViewPos;
	vec3 v_TangentFragPos;
} vs_out;

void main()
{
	gl_Position = pvMat * modelMat * vec4(a_Position, 1.0f);
	vs_out.v_FragPos = modelMat * vec4(a_Position, 1.0f);
	vs_out.v_TexCoord = a_TexCoord;
	
	vec3 T = normalize(a_Tangent);
	vec3 B = normalize(a_Bitangent);
	vec3 N = normalize(a_Normal);

	mat3 TBN = normalMat * mat3(a_Tangent, a_Bitangent, a_Normal); // from model space to tangent space
	mat3 tsMat = inverse(TBN);

	vs_out.v_TangentLightPos = tsMat * lightPos;
	vs_out.v_TangentViewPos = tsMat * viewPos;
	vs_out.v_TangentFragPos = tsMat * (vs_out.v_FragPos.xyz / vs_out.v_FragPos.w);
}