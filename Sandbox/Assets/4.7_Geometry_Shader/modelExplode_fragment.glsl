#version 460 core
in GS_OUT
{
	in vec3 v_Normal;
	in vec3 v_FragPos;
	in vec2 v_TexCoord;
} fs_in;

out vec4 color;

uniform vec3 observePos; 

struct Model
{
	sampler2D texture_diffuse0;
	sampler2D texture_specular0;
};
uniform Model model;

struct FlashLight
{
	vec3 position;
	vec3 direction;

	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;

	float innerCutOff;
	float outerCutOff;
};
uniform FlashLight flashLight;

vec3 CalFlashLight(FlashLight light, vec3 norm, vec3 observeDir)
{
	vec3 diffuseColoro = texture(model.texture_diffuse0, fs_in.v_TexCoord).rgb;
	vec3 specularColoro = texture(model.texture_specular0, fs_in.v_TexCoord).rgb;

	vec3 diffuseColor = pow(diffuseColoro, vec3(2.2f));
	vec3 specularColor = pow(specularColoro, vec3(2.2f));

	vec3 ambient = diffuseColor * light.ambientColor;

	vec3 lightDir = normalize(light.position - fs_in.v_FragPos);
	float angle = dot(lightDir, -light.direction);

	if (angle <= light.outerCutOff)
	{
		return ambient;
	}
	else
	{
		vec3 diffuse = diffuseColor * max(dot(lightDir, norm), 0.0f) * light.diffuseColor;
		
		
		vec3 halfWay = normalize(observeDir + lightDir);
		vec3 specular = specularColor * pow(max(dot(halfWay, norm), 0.0f), 128.0f) * light.specularColor;
		
		if (angle > light.innerCutOff)
		{
			return ambient + diffuse + specular;
		}
		else
		{
			float intensity = smoothstep(light.outerCutOff, light.innerCutOff, angle);
			return ambient + (diffuse + specular) * intensity;
		}
	}
}

void main()
{
	vec3 norm = normalize(fs_in.v_Normal);
	vec3 observeDir = normalize(observePos - fs_in.v_FragPos);
	color = vec4(CalFlashLight(flashLight, norm, observeDir), 1.0f);
	color = pow(color, vec4(1.0f / 2.2f));
}