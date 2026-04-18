#pragma once

#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
    public:
    
        glm::vec3 position; 
        float yaw= -90.0f;  // Regarde vers -Z par défaut
        float pitch = 0.0f;
        float speed = 0.05f;
        float sensitivity = 0.1f;

        Camera(glm::vec3 pos = {0, 0, 3}) : position(pos) { updateVectors(); }

        glm::mat4 getViewMatrix() const {
            return glm::lookAt(position, position + front, up);
        }

        void processMouse(loat dx, float dy)
        {
            yam += dx * sensitivity;
            pitch = glm::clamp(pitch - dy * sensitivity, -89.0f, 89.0f);
            updateVectors();
        }

        void processKeybord(const bool* keys)
        {
            if (keys[SDL_SCANCODE_Z]) position += front * speed;  // go forward
            if (keys[SDL_SCANCODE_S]) position -= front * speed;  // backward
            if (keys[SDL_SCANCODE_Q]) position -= right * speed;  // left
            if (keys[SDL_SCANCODE_D]) position += right * speed;  // right
        }
    
    private:
        glm::vec3 front = {0, 0, -1};
        glm::vec3 right, up;

        
};