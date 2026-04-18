#version 330 core

// Input vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

// Uniform variables (mêmes pour tous les vertices)
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertexColor; // Couleur à passer au fragment shader

void main()
{
    // Transforme la position du vertex
    gl_Position = projection * view * model * vec4(position, 1.0);
    vertexColor = color;
}
