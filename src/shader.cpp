#include "shader.hpp"
#include "shader_loader.hpp"
#include <iostream>

Shader::Shader(const char* vertex_path, const char* fragment_path) {
    id = load_shader_program(vertex_path, fragment_path);
    if (id == 0) {
        std::cerr << "Shader failed to load: " << vertex_path << " or " << fragment_path << "\n";
    }
}

Shader::~Shader() {
    if (id != 0) {
        glDeleteProgram(id);
    }
}

GLint Shader::getUniformLocation(const std::string& name) {
    if (uniform_locations.find(name) != uniform_locations.end()) {
        return uniform_locations[name];
    }
    GLint loc = glGetUniformLocation(id, name.c_str());
    uniform_locations[name] = loc;
    return loc;
}
