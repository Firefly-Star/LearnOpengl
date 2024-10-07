#version 330 core
layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec3 a_Color;
layout (location = 2) in vec2 a_Offset;

out vec3 v_Color;

uniform vec2 offsets[100];

void main()
{
    gl_Position = vec4(a_Position * (gl_InstanceID / 100.0f) + a_Offset, 0.0, 1.0);
    v_Color = a_Color;
}