#include <fstream>
#include <iostream>

std::string read_file(const char* filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Could not open shader file: " << filepath << "\n";
        return "";
    }
    
    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + "\n";
    }
    file.close();
    return content;
}