// spaceship.h
#pragma once
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Spaceship {
public:
    Spaceship();
    void Update(float deltaTime);
    void Draw(GLuint shader) const;

    void ProcessKeyInput(int key, int action);
    void createModel();  // ✅ Aquí, en la sección pública

    glm::vec3 position, velocity, direction;
    float speed;

private:
    GLuint VAO, VBO;
    int vertexCount = 0;
};

