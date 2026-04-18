#pragma once
#include <string>
#include <unordered_map>
#include "glad/gl.hpp"

class Shader {
public:
    Shader(const char* vertex_path, const char* fragment_path);
    ~Shader();

    // Prevent copying to avoid double-deletion of OpenGL resource
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    void use() const { glUseProgram(id); }

    GLint getUniformLocation(const std::string& name);

    unsigned int getId() const { return id; }

private:
    unsigned int id;
    std::unordered_map<std::string, GLint> uniform_locations;
};
