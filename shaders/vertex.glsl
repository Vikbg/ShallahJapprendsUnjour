#version 330 core

// Input vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

// Uniform variables (mêmes pour tous les vertices)
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Color; // Couleur à passer au fragment shader

void main()
{
    vec4 worldPos = model * vec4(position, 1.0);
    
    // Transforme la position du vertex
    gl_Position = projection * view * worldPos;
    Color = color;
}
