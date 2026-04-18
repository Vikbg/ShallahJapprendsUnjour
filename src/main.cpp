#include <iostream>
#include <memory>
#include "glad/gl.hpp"
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "shader_loader.hpp"
#include "config.hpp"
#include "engine.hpp"

int main()
{
    try
    {
        Engine engine;

        // for capture the mouse pos ect and hide it
        SDL_SetWindowRelativeMouseMode(engine.getWindow(), true);

        Uint64 lastTime = SDL_GetTicks();
        int frameCount = 0;
        float fps = 0.0f;

        bool done = false;
        while (!done)
        {
            SDL_Event event;

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_EVENT_QUIT)
                {
                    done = true;
                }
                if (event.type == SDL_EVENT_WINDOW_RESIZED ||
                    event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
                        engine.updateViewport();
                }

                if (event.type == SDL_EVENT_MOUSE_MOTION)
                {
                    engine.camera.processMouse(event.motion.xrel, event.motion.yrel);
                }
            }

            Uint64 currentTime = SDL_GetTicks();
            frameCount++;

            if (currentTime - lastTime >= 1000)
            {
                fps = frameCount / ((currentTime - lastTime) / 1000.0f);
                frameCount = 0;
                lastTime = currentTime;

                SDL_SetWindowTitle(engine.getWindow(), (std::string(WINDOW_TITLE) + " - FPS: " + std::to_string(fps)).c_str());
            }

            engine.camera.processKeybord((SDL_GetKeyboardState((nullptr))));

            engine.render(currentTime);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}