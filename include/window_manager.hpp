#pragma once

#include "glad/gl.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL3/SDL.h>
#include "config.hpp"

class Window
{
public:
    SDL_Window *window = nullptr;
    glm::mat4 projection;

    Window();
    ~Window();

    void updateViewport();

    SDL_Window* getWindow() const;

private:
    void init_sdl();
};