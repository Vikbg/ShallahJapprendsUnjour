#version 330 core

// Input vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;

// Uniform variables (mêmes pour tous les vertices)
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// pos and normals in world-space
out vec3 FragPos;
out vec3 Normal;
out vec3 Color; // Couleur à passer au fragment shader

void main()
{
    vec4 worldPos = model * vec4(position, 1.0);
    FragPos = worldPos.xyz;

    Normal = mat3(transpose(inverse(model))) * normal;

    // Transforme la position du vertex
    gl_Position = projection * view * worldPos;
    Color = color;
}
