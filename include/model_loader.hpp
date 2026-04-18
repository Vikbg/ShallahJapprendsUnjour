#pragma once
#include <vector>
#include <string>

struct Vertex {
    float x, y, z;
    float r, g, b;
};

class ModelLoader {
public:
    static std::vector<Vertex> loadOBJ(const std::string& filename);
};
