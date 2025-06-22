#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "spaceship.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

Spaceship::Spaceship() {
    position = glm::vec3(0.0f, 0.0f, 1000.0f);
    velocity = glm::vec3(0.0f);
    direction = glm::vec3(0.0f, 0.0f, -1.0f);
    speed = 100.0f;

}

void Spaceship::createModel() {
    std::vector<float> vertices = {
        // base
        -5.0f,  0.0f, -5.0f,
         5.0f,  0.0f, -5.0f,
         0.0f,  0.0f,  5.0f,
        // pirámide
         0.0f,  10.0f,  0.0f
    };


    float shipVertices[] = {
        // base
        vertices[0], vertices[1], vertices[2],
        vertices[3], vertices[4], vertices[5],
        vertices[6], vertices[7], vertices[8],
        // lados
        vertices[0], vertices[1], vertices[2], vertices[9], vertices[10], vertices[11], vertices[3], vertices[4], vertices[5],
        vertices[3], vertices[4], vertices[5], vertices[9], vertices[10], vertices[11], vertices[6], vertices[7], vertices[8],
        vertices[6], vertices[7], vertices[8], vertices[9], vertices[10], vertices[11], vertices[0], vertices[1], vertices[2],
    };

    vertexCount = 18;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(shipVertices), shipVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
}

void Spaceship::Draw(GLuint shader) const {
    GLint modelLoc = glGetUniformLocation(shader, "model");
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    model = glm::rotate(model, glm::atan(direction.x, -direction.z), glm::vec3(0,1,0));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    GLint colorLoc = glGetUniformLocation(shader, "objectColor");
    glUniform4f(colorLoc, 1.0f, 0.3f, 0.3f, 1.0f); // rojo claro

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}

void Spaceship::Update(float dt) {
    position += velocity * dt;
}

void Spaceship::ProcessKeyInput(int key, int action) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        glm::vec3 right = glm::normalize(glm::cross(direction, glm::vec3(0, 1, 0)));
        glm::vec3 up = glm::vec3(0, 1, 0);

        if (key == GLFW_KEY_W) velocity = direction * speed;
        if (key == GLFW_KEY_S) velocity = -direction * speed;
        if (key == GLFW_KEY_A) velocity = -right * speed;
        if (key == GLFW_KEY_D) velocity = right * speed;
        if (key == GLFW_KEY_Q) velocity = up * speed;
        if (key == GLFW_KEY_E) velocity = -up * speed;
    }
    if (action == GLFW_RELEASE) {
        velocity = glm::vec3(0);
    }
}
