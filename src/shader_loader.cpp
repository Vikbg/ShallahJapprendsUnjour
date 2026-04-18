#include <string>
#include <fstream>
#include <iostream>
#include "glad/gl.hpp"
#include "read_file.hpp"

unsigned int compile_shader(const char* source, GLenum type)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << "\n";
        return 0;
    }
    
    return shader;
}

unsigned int create_program(unsigned int vertex_shader, unsigned int fragment_shader)
{
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Program linking failed: " << infoLog << "\n";
        return 0;
    }
    
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    return program;
}

unsigned int load_shader_program(const char* vertex_path, const char* fragment_path)
{
    std::string vertex_source = read_file(vertex_path);
    std::string fragment_source = read_file(fragment_path);
    
    if (vertex_source.empty() || fragment_source.empty()) {
        return 0;
    }
    
    unsigned int vertex_shader = compile_shader(vertex_source.c_str(), GL_VERTEX_SHADER);
    unsigned int fragment_shader = compile_shader(fragment_source.c_str(), GL_FRAGMENT_SHADER);
    
    if (vertex_shader == 0 || fragment_shader == 0) {
        return 0;
    }
    
    return create_program(vertex_shader, fragment_shader);
}
