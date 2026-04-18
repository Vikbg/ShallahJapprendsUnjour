#pragma once
#include <vector>
#include <string>

struct Vertex {
    float x, y, z;      // pos
    float nx, ny, nz;   // normals
    float r, g, b;      // color
};

class ModelLoader {
public:
    static std::vector<Vertex> loadOBJ(const std::string& filename);
};
