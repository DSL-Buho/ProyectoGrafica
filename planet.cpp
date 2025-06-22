// planet.cpp
#include "globals.h"
#include "planet.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

// Constantes de escala
static constexpr float DIST_SCALE = 1.0f / 5.0e7f;  // 1 unidad = 50 millones km
static constexpr float BODY_SCALE = 400.0f;         // escala visual para cuerpos

// Constructor: calcula radios, crea esfera y buffers
CelestialBody::CelestialBody(glm::vec3 center,
                             float oRadius_km,
                             float mass,
                             float density,
                             glm::vec4 color)
    : orbitCenter(center),
      mass(mass),
      density(density),
      color(color),
      orbitAngle(0.0f),
      selfRotationAngle(0.0f),
      precession(0.0f),
      nutation(0.0f)
{
    radius_km = std::cbrt((3.0f * mass) / (4.0f * glm::pi<float>() * density));
    scaledRadius = radius_km * BODY_SCALE * DIST_SCALE;
    scaledOrbitRadius = oRadius_km * DIST_SCALE;

    auto vertices = CreateSphereVertices(scaledRadius, 36, 36);
    vertexCount = vertices.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glBindVertexArray(0);
}

// Generación de esfera: triangulación UV
std::vector<float> CelestialBody::CreateSphereVertices(float r, int stacks, int sectors) {
    std::vector<float> vertices;
    vertices.reserve(stacks * sectors * 6 * 3);
    for (int i = 0; i < stacks; ++i) {
        float theta1 = glm::pi<float>() * i / stacks;
        float theta2 = glm::pi<float>() * (i + 1) / stacks;
        for (int j = 0; j < sectors; ++j) {
            float phi1 = 2.0f * glm::pi<float>() * j / sectors;
            float phi2 = 2.0f * glm::pi<float>() * (j + 1) / sectors;

            glm::vec3 v1 = sphericalToCartesian(r, theta1, phi1);
            glm::vec3 v2 = sphericalToCartesian(r, theta1, phi2);
            glm::vec3 v3 = sphericalToCartesian(r, theta2, phi1);
            glm::vec3 v4 = sphericalToCartesian(r, theta2, phi2);

            // Triángulo 1
            vertices.insert(vertices.end(), {v1.x, v1.y, v1.z,
                                             v2.x, v2.y, v2.z,
                                             v3.x, v3.y, v3.z});
            // Triángulo 2
            vertices.insert(vertices.end(), {v2.x, v2.y, v2.z,
                                             v4.x, v4.y, v4.z,
                                             v3.x, v3.y, v3.z});
        }
    }
    return vertices;
}

// Actualiza ángulos de animación
void CelestialBody::UpdateAnimation(float dt) {
    orbitAngle += orbitSpeed * dt;
    selfRotationAngle += selfRotationSpeed * dt;
    precession += precessionSpeed * dt;
    nutation += nutationSpeed * dt;
}

// Calcula matriz modelo: traslación orbital, rotación axial, precesión, etc.
glm::mat4 CelestialBody::GetModelMatrix() const {
    glm::mat4 model(1.0f);
    model = glm::translate(model, orbitCenter); // centro orbital (usualmente el Sol)
    model = glm::rotate(model, precession, glm::vec3(0, 1, 0));
    model = glm::rotate(model, nutationAmplitude * sin(nutation), glm::vec3(0, 0, 1));
    model = glm::translate(model, glm::vec3(
        scaledOrbitRadius * cos(orbitAngle),
        0.0f,
        scaledOrbitRadius * sin(orbitAngle)
    ));
    model = glm::rotate(model, selfRotationAngle, glm::vec3(0, 1, 0));
    return model;
}

// Renderiza el planeta
void CelestialBody::Draw(GLuint shader) const {
    GLint colorLoc = glGetUniformLocation(shader, "objectColor");
    glUniform4fv(colorLoc, 1, &color[0]);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount / 3);
    glBindVertexArray(0);
}

// Convierte coordenadas esféricas a cartesianas
glm::vec3 CelestialBody::sphericalToCartesian(float r, float theta, float phi) {
    float x = r * sin(theta) * cos(phi);
    float y = r * cos(theta);
    float z = r * sin(theta) * sin(phi);
    return glm::vec3(x, y, z);
}
