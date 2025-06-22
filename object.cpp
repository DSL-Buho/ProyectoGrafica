// object.cpp
#include "globals.h"
#include "object.h"
#include <cmath>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#define M_PI 3.14159265358979323846

// Constructor: calcula radio, crea geometría y buffers
Object::Object(glm::vec3 initPosition, glm::vec3 initVelocity, float mass, float density)
    : position(initPosition),
      velocity(initVelocity),
      mass(mass),
      density(density),
      Initalizing(false),
      Launched(false),
      target(false)
{
    radius = std::cbrt((3.0f * mass) / (4.0f * M_PI * density)) / 100000.0f;

    std::vector<float> vertices = Draw();
    vertexCount = vertices.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}

// Genera vértices de una esfera (triángulos) para glDrawArrays
std::vector<float> Object::Draw() {
    std::vector<float> vertices;
    const int STACKS = 10, SECTORS = 10;

    for(int i = 0; i < STACKS; ++i) {
        float theta1 = M_PI * i / STACKS;
        float theta2 = M_PI * (i + 1) / STACKS;
        for(int j = 0; j < SECTORS; ++j) {
            float phi1 = 2.0f * M_PI * j / SECTORS;
            float phi2 = 2.0f * M_PI * (j + 1) / SECTORS;

            glm::vec3 v1 = sphericalToCartesian(radius, theta1, phi1);
            glm::vec3 v2 = sphericalToCartesian(radius, theta1, phi2);
            glm::vec3 v3 = sphericalToCartesian(radius, theta2, phi1);
            glm::vec3 v4 = sphericalToCartesian(radius, theta2, phi2);

            // Triángulo 1
            vertices.insert(vertices.end(), { v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z });
            // Triángulo 2
            vertices.insert(vertices.end(), { v2.x, v2.y, v2.z, v4.x, v4.y, v4.z, v3.x, v3.y, v3.z });
        }
    }
    return vertices;
}

// Dibuja la esfera
void Object::DrawRender() const {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount / 3);
    glBindVertexArray(0);
}

// Actualiza posición
void Object::UpdatePos() {
    position += velocity / 94.0f;
    radius = std::cbrt((3.0f * mass) / (4.0f * M_PI * density)) / 100000.0f;
}

// Actualiza VBO cuando cambia el radio
void Object::UpdateVertices() {
    std::vector<float> vertices = Draw();
    vertexCount = vertices.size();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices.data(), GL_STATIC_DRAW);
}

// Devuelve posición
glm::vec3 Object::GetPos() const {
    return position;
}

// Aplica aceleración
void Object::accelerate(float x, float y, float z) {
    velocity += glm::vec3(x, y, z) / 96.0f;
}

// Detecta colisión (retorna factor de rebote)
float Object::CheckCollision(const Object& other) const {
    float dist = glm::length(other.position - position);
    if ((other.radius + radius) > dist) {
        return -0.2f;
    }
    return 1.0f;
}

// Conversión esférica
glm::vec3 Object::sphericalToCartesian(float r, float theta, float phi) {
    return {
        r * sin(theta) * cos(phi),
        r * cos(theta),
        r * sin(theta) * sin(phi)
    };
}

glm::mat4 Object::GetModelMatrix() const {
    return glm::translate(glm::mat4(1.0f), position);
}

void Object::Draw(GLuint shader) const {
    GLint colorLoc = glGetUniformLocation(shader, "objectColor");
    glUniform4fv(colorLoc, 1, &color[0]);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount / 3);
    glBindVertexArray(0);
}