#version 330 core
out vec4 FragColor;

in vec2 v_TexCoord;

struct Model
{
    sampler2D texture_diffuse0;
};
uniform Model model;

void main()
{    
    FragColor = texture(model.texture_diffuse0, v_TexCoord);
}