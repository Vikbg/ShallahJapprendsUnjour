#include "glad/gl.hpp"
#include <memory>
#include "shader.hpp"
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "config.hpp"
#include "model_loader.hpp"
#include "camera.hpp"
#include "window_manager.hpp"


class Engine
{
public:
    Window window;
    SDL_GLContext gl_context = nullptr;
    unsigned int VAO = 0, VBO = 0;
    int vertexCount = 0;
    std::unique_ptr<Shader> shader;
    GLint modelLoc, viewLoc, projLoc, timeLoc, lightPosLoc, lightColorLoc, viewPosLoc;
    Camera camera;

    Engine()
    {
        init_opengl_glad();
        setup_geometry();
        init_shaders();
    }

    ~Engine()
    {
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (VBO) glDeleteBuffers(1, &VBO);
        if (gl_context) SDL_GL_DestroyContext(gl_context);
        SDL_Quit();
    }

    void render(Uint64 currentTime)
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->use();

        glm::vec3 lightPos   = glm::vec3(2.0f, 4.0f, 3.0f);
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::mat4 model = glm::mat4(1.0f);
        
        model = glm::rotate(model, glm::radians(currentTime * 0.09f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(currentTime * 0.01f), glm::vec3(0.0f, 40.0f, 0.0f));
        model = glm::rotate(model, glm::radians(currentTime * 0.12f), glm::vec3(1.0f, 0.0f, 0.0f));
        

        glm::mat4 view = camera.getViewMatrix();
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -6.0f));

        glUniform3fv(lightPosLoc,   1, glm::value_ptr(lightPos));
        glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
        glUniform3fv(viewPosLoc,    1, glm::value_ptr(camera.position));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(window.projection));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        glBindVertexArray(0);

        SDL_GL_SwapWindow(window.getWindow());
    }

private:

    void init_opengl_glad()
    {
        gl_context = SDL_GL_CreateContext(window.getWindow());
        if (!gl_context)
            throw std::runtime_error(std::string("OpenGL Context creation failed: ") + SDL_GetError());

        if (!gladLoaderLoadGL())
            throw std::runtime_error("Failed to initialize GLAD");

        glEnable(GL_DEPTH_TEST);
    }

    void setup_geometry()
    {
        std::vector<Vertex> modelVertices = ModelLoader::loadOBJ("models/kar98k.obj");
        if (modelVertices.empty()) {
            throw std::runtime_error("Failed to load model.obj or model is empty");
        }
        vertexCount = static_cast<int>(modelVertices.size());

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(Vertex), modelVertices.data(), GL_STATIC_DRAW);

        // pos
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);

        // normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, nx));
        glEnableVertexAttribArray(1);

        // color
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, r));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    void init_shaders()
    {
        shader = std::make_unique<Shader>("shaders/vertex.glsl", "shaders/fragment.glsl");

        modelLoc = shader->getUniformLocation("model");
        viewLoc = shader->getUniformLocation("view");
        projLoc = shader->getUniformLocation("projection");
        timeLoc = shader->getUniformLocation("time");

        lightPosLoc     = shader->getUniformLocation("lightPos");
        lightColorLoc   = shader->getUniformLocation("lightColor");
        viewPosLoc      = shader->getUniformLocation("viewPos");        

        window.projection = glm::perspective(glm::radians(90.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
    }
};