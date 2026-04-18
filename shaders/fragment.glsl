#version 330 core

in vec3 Color;
in vec3 FragPos;

// Output du shader
out vec4 FragColor;

void main()
{
    FragColor = vec4(Color, 1.0);
}
