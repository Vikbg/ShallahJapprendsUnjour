#include "model_loader.hpp"
#include "tinyobjloader/tiny_obj_loader.h"
#include <fstream>
#include <sstream>
#include <iostream>

static std::vector<Vertex> loadPlainVertexList(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return {};
    }

    std::vector<Vertex> vertices;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);
        float x, y, z, r, g, b;
        if (ss >> x >> y >> z >> r >> g >> b) {
            vertices.push_back({x, y, z, r, g, b});
            continue;
        }

        ss.clear();
        ss.str(line);
        std::string prefix;
        if (ss >> prefix) {
            if (prefix == "v") {
                if (ss >> x >> y >> z >> r >> g >> b) {
                    vertices.push_back({x, y, z, r, g, b});
                    continue;
                }
                ss.clear();
                ss.str(line);
                ss >> prefix;
                if (ss >> x >> y >> z) {
                    vertices.push_back({x, y, z, 1.0f, 1.0f, 1.0f});
                    continue;
                }
            }
        }
    }
    return vertices;
}

std::vector<Vertex> ModelLoader::loadOBJ(const std::string& filename) {
    tinyobj::ObjReaderConfig reader_config;
    reader_config.vertex_color = true;  // Enable vertex color loading

    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(filename, reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjLoader error: " << reader.Error() << "\n";
        }
        auto fallbackVertices = loadPlainVertexList(filename);
        if (!fallbackVertices.empty()) {
            return fallbackVertices;
        }
        return {};
    }

    if (!reader.Warning().empty()) {
        std::cerr << "TinyObjLoader warning: " << reader.Warning() << "\n";
    }

    const tinyobj::attrib_t& attrib = reader.GetAttrib();
    const std::vector<tinyobj::shape_t>& shapes = reader.GetShapes();
    const std::vector<tinyobj::material_t>& materials = reader.GetMaterials();
    const bool hasVertexColors = attrib.colors.size() == attrib.vertices.size();

    std::cerr << "Materials loaded: " << materials.size() << "\n";
    for (size_t i = 0; i < materials.size(); i++) {
        std::cerr << "  Material " << i << " (" << materials[i].name << "): Kd = (" 
                  << materials[i].diffuse[0] << ", " << materials[i].diffuse[1] << ", " 
                  << materials[i].diffuse[2] << ")\n";
    }

    std::vector<Vertex> vertices;
    vertices.reserve(shapes.size() * 3);

    for (const auto& shape : shapes) {
        const auto& mesh = shape.mesh;
        size_t vertex_offset = 0;
        
        std::cerr << "Shape: " << shape.name << " has " << mesh.num_face_vertices.size() << " faces\n";
        
        for (size_t f = 0; f < mesh.num_face_vertices.size(); f++) {
            int fv = mesh.num_face_vertices[f];
            int mat_id = mesh.material_ids[f];
            
/*             if (f < 10) {  // Print first 10 faces for debugging
                std::cerr << "  Face " << f << ": " << fv << " vertices, material_id=" << mat_id;
                if (mat_id >= 0 && mat_id < static_cast<int>(materials.size())) {
                    std::cerr << " (" << materials[mat_id].name << ") Kd=(" 
                              << materials[mat_id].diffuse[0] << ", " 
                              << materials[mat_id].diffuse[1] << ", " 
                              << materials[mat_id].diffuse[2] << ")";
                }
                std::cerr << "\n";
            }
             */
            for (int v = 0; v < fv; v++) {
                const auto& index = mesh.indices[vertex_offset + v];
                
                if (index.vertex_index < 0) {
                    continue;
                }

                const float vx = attrib.vertices[3 * index.vertex_index + 0];
                const float vy = attrib.vertices[3 * index.vertex_index + 1];
                const float vz = attrib.vertices[3 * index.vertex_index + 2];

                float r = 1.0f;
                float g = 1.0f;
                float b = 1.0f;
                
                // First try vertex colors
                if (hasVertexColors) {
                    r = attrib.colors[3 * index.vertex_index + 0];
                    g = attrib.colors[3 * index.vertex_index + 1];
                    b = attrib.colors[3 * index.vertex_index + 2];
                } 
                // Otherwise use material diffuse color
                else if (mat_id >= 0 && mat_id < static_cast<int>(materials.size())) {
                    r = materials[mat_id].diffuse[0];
                    g = materials[mat_id].diffuse[1];
                    b = materials[mat_id].diffuse[2];
                }

                vertices.push_back({vx, vy, vz, r, g, b});
            }
            vertex_offset += fv;
        }
    }

    if (vertices.empty()) {
        auto fallbackVertices = loadPlainVertexList(filename);
        if (!fallbackVertices.empty()) {
            return fallbackVertices;
        }
        std::cerr << "No valid vertices found in OBJ file: " << filename << "\n";
    }

    return vertices;
}
