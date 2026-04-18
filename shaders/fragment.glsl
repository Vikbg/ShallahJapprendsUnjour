#version 330 core

in vec3 Color;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;      // light pos in world-space
uniform vec3 lightColor;    // rgb color
uniform vec3 viewPos;       // camera pos like yk

// Output du shader
out vec4 FragColor;

// I will use the phong model for light

void main()
{
    // ambient
    float ambientStrengh = 0.15;
    vec3 ambient = ambientStrengh * lightColor;

    // diffuse
    vec3 norm     = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff    = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = diff * lightColor;

    // specular
    float specStrength = 0.5;
    vec3 viewDir       = normalize(viewPos - FragPos);
    vec3 reflectDir    = reflect(-lightDir, norm);
    float spec         = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular      = specStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * Color;

    FragColor = vec4(result, 1.0);
}
