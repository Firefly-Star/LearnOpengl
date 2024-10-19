#version 460 core

in VS_OUT
{
	vec3 v_Normal;
	vec2 v_TexCoord;
	vec4 v_FragPos;
	vec4 v_FragPosInLightClipSpace;
} fs_in;

out vec4 color;

uniform sampler2D depthMap;
uniform sampler2D tex;
uniform vec3 viewPos;
uniform vec3 lightDir;



//float CalShadow()
//{
//    vec3 lightSpacePos = fs_in.v_FragPosInLightClipSpace.xyz / fs_in.v_FragPosInLightClipSpace.w;
//    lightSpacePos = lightSpacePos * 0.5 + 0.5;
//
//    float closestDepth = texture(depthMap, lightSpacePos.xy).r;
//    
//    float currentDepth = lightSpacePos.z;
//
//    float bias = 0.005f;
//    float shadow;
//    if (currentDepth > 1.0)
//    {
//        shadow = 1.0f;
//    }
//    else
//    {
//        shadow = (currentDepth > closestDepth + bias) ? 0.0f : 1.0f;
//    }
//    return shadow;
//}

float CalShadow()
{
    vec3 lightSpacePos = fs_in.v_FragPosInLightClipSpace.xyz / fs_in.v_FragPosInLightClipSpace.w;
    lightSpacePos = lightSpacePos * 0.5 + 0.5;

    float currentDepth = lightSpacePos.z;
    vec2 pixelSize = vec2(1.0f) / textureSize(depthMap, 0);
    float shadow;
    float bias = 0.005f;
    if (currentDepth > 1.0)
    {
        shadow = 9.0f;
    }
    else
    {
        for (int x = -1; x <= 1; ++x)
        {
            for (int y = -1; y <= 1; ++y)
            {
                float closestDepth = texture(depthMap, lightSpacePos.xy + vec2(x, y) * pixelSize).r;
                shadow += (currentDepth > closestDepth + bias) ? 0.0f : 1.0f;
            }
        }
    }
    
    return shadow / 9.0f;
}

void main()
{
    float shadow = CalShadow();

    vec3 fragPos = fs_in.v_FragPos.xyz / fs_in.v_FragPos.w;
    vec3 lightDir2 = -normalize(lightDir);
    vec3 norm = normalize(fs_in.v_Normal);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfWay = normalize(viewDir + lightDir2);

    float ambientStrength = 0.1f;
    float diffuseStrength = 0.5f;
    float specularStrength = 1.0f;

    vec3 texColor = pow(texture(tex, fs_in.v_TexCoord).rgb, vec3(2.2f));

    vec3 ambientColor = texColor * ambientStrength;
    vec3 diffuseColor = texColor * diffuseStrength * max(dot(norm, lightDir2), 0.0f);
    float shininess = 512.0f;
    vec3 specularColor = vec3(1.0f) * specularStrength * pow(max(dot(halfWay, norm), 0.0f), shininess);

    vec3 result = ambientColor + (diffuseColor + specularColor) * shadow;

    color = vec4(result, 1.0f);
    color = pow(color, vec4(1.0f / 2.2f));
}