#version 460 core

in vec2 v_TexCoord;
uniform sampler2D tex;

out vec4 color;

float Linearlize(float depth, float near, float far)
{
    return near * far / (far + depth * (near - far));
}

void main()
{
    float depth = texture(tex, v_TexCoord).r;
    float near = 0.1;
    float far = 100.0;
    float linearDepth = Linearlize(depth, near, far);
    color = vec4(linearDepth * vec3(1.0f) / far, 1.0f);
}
