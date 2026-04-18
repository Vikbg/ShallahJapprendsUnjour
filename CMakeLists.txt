cmake_minimum_required(VERSION 3.20)
project(ShallahJapprendsUnJour LANGUAGES CXX)

# Use the C++23 standard.
set(CMAKE_CXX_STANDARD 23)

# Require the compiler to support the requested standard.
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable compiler-specific extensions.
set(CMAKE_CXX_EXTENSIONS OFF)

# Set OpenGL preference to GLVND to avoid CMP0072 warning
set(OpenGL_GL_PREFERENCE GLVND)

find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)

# OpenGL triangle executable.
add_executable(opengl_triangle
  src/main.cpp
  src/gl.cpp
  src/shader.cpp
  src/shader_loader.cpp
  src/read_file.cpp
  src/model_loader.cpp
  src/vendors.cpp
  src/window_manager.cpp
)

target_include_directories(opengl_triangle PRIVATE include)

# Link SDL3 and system OpenGL.
target_link_libraries(opengl_triangle PRIVATE SDL3::SDL3 OpenGL::GL)
