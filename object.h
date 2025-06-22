// object.h
#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Object {
public:
    GLuint VAO = 0, VBO = 0;
    glm::vec3 position, velocity;
    size_t vertexCount;
    glm::vec4 color = glm::vec4(1,0,0,1);

    bool Initalizing, Launched, target;
    float mass, density, radius;
    glm::vec3 LastPos;
    glm::mat4 GetModelMatrix() const;
    void Draw(GLuint shader) const;

    Object(glm::vec3 initPosition, glm::vec3 initVelocity, float mass, float density = 3344.0f);

    void DrawRender() const;
    void UpdatePos();
    void UpdateVertices();
    glm::vec3 GetPos() const;
    void accelerate(float x, float y, float z);
    float CheckCollision(const Object& other) const;

    static glm::vec3 sphericalToCartesian(float r, float theta, float phi);

private:
    std::vector<float> Draw();
};
