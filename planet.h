#ifndef PLANET_H
#define PLANET_H

#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>

class CelestialBody {
public:
    // Constructor
    CelestialBody(glm::vec3 center,
                  float orbitRadiusKm,
                  float mass,
                  float density,
                  glm::vec4 color = glm::vec4(1.0f));

    // --- VARIABLES NECESARIAS PARA ANIMACIÓN ---
    glm::vec3 position;     // ← Posición actual del planeta
    float orbitRadius;      // ← Radio orbital en escena

    // Velocidades configurables desde fuera
    float angularVelocity = 0.0f; // No se usa directamente, puedes borrarlo si no lo necesitas

    void UpdateAnimation(float dt);        // Actualiza ángulos
    glm::mat4 GetModelMatrix() const;      // Devuelve matriz de transformación
    void Draw(GLuint shader) const;        // Dibuja el planeta

    // --- Setters de velocidades ---
    void SetOrbitSpeed(float radPerSec)         { orbitSpeed = radPerSec; }
    void SetSelfRotationSpeed(float radPerSec)  { selfRotationSpeed = radPerSec; }
    void SetPrecessionSpeed(float radPerSec)    { precessionSpeed = radPerSec; }
    void SetNutationSpeed(float radPerSec)      { nutationSpeed = radPerSec; }
    void SetNutationAmplitude(float rad)        { nutationAmplitude = rad; }

    // OpenGL buffers
    GLuint VAO = 0, VBO = 0;

private:
    size_t vertexCount = 0;
    glm::vec4 color;
    glm::vec3 orbitCenter;

    float mass;
    float density;
    float radius_km;
    float scaledRadius;
    float scaledOrbitRadius;

    // Estados de animación
    float orbitAngle = 0.0f;
    float orbitSpeed = 0.0f;

    float selfRotationAngle = 0.0f;
    float selfRotationSpeed = 0.0f;

    float precession = 0.0f;
    float precessionSpeed = 0.0f;

    float nutation = 0.0f;
    float nutationSpeed = 0.0f;
    float nutationAmplitude = 0.0f;

    // Geometría
    std::vector<float> CreateSphereVertices(float radius, int stacks, int sectors);
    glm::vec3 sphericalToCartesian(float r, float theta, float phi);
};

#endif // PLANET_H
