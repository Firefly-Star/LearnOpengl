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

float Linearlize(float depth, float near, float far) // from 0-1 to linear near-far
{
    return near * far / (far + depth * (near - far));
}

void main()
{
    vec3 lightSpacePos = fs_in.v_FragPosInLightClipSpace.xyz / fs_in.v_FragPosInLightClipSpace.w;
    lightSpacePos = lightSpacePos * 0.5 + 0.5;

    float closestDepth = texture(depthMap, lightSpacePos.xy).r;
    
    float currentDepth = lightSpacePos.z;
    
    float near = 0.1f;
    float far = 100.0f;

    closestDepth = Linearlize(closestDepth, near, far);
    currentDepth = Linearlize(currentDepth, near, far);

    float bias = 0.1;
    bool shadow;
    if (currentDepth > far)
    {
        shadow = false;
    }
    else
    {
        shadow = currentDepth > closestDepth + bias;
    }
    
    color = shadow ? vec4(0.0f, 0.0f, 0.0f, 1.0f) : vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //color = vec4(currentDepth * vec3(1.0f, 1.0f, 1.0f), 1.0f);
}