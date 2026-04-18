#version 330 core

// Output du shader
out vec4 FragColor;
in vec3 vertexColor;

void main()
{
    FragColor = vec4(vertexColor, 1.0);
}
