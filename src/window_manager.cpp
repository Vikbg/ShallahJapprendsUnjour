#include "window_manager.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <SDL3/SDL.h>
#include <stdexcept>

Window::Window()
{
    init_sdl();
}

Window::~Window()
{
    if (window)
        SDL_DestroyWindow(window);
}

void Window::updateViewport()
{
    int width = 0;
    int height = 0;
    SDL_GetWindowSizeInPixels(window, &width, &height);
    glViewport(0, 0, width, height);
    projection = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.1f, 100.0f);
}

void Window::init_sdl()
{
    if (!SDL_Init(SDL_FLAGS))
        throw std::runtime_error(std::string("SDL initialization failed: ") + SDL_GetError());

    window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_FLAGS);
    if (!window)
        throw std::runtime_error(std::string("Could not create window: ") + SDL_GetError());
}