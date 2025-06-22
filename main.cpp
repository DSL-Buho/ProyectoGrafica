#include "globals.h"
#include "object.h"
#include "planet.h"
#include "functions.h"
#include "spaceship.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const char* vertexSrc = R"glsl(
#version 330 core
layout(location=0) in vec3 aPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)glsl";

const char* fragmentSrc = R"glsl(
#version 330 core
out vec4 FragColor;
uniform vec4 objectColor;
void main(){
    FragColor = objectColor;
}
)glsl";

Spaceship space;


int main() {

    GLFWwindow* window = StartGLU();
    if (!window) return -1;

    space.createModel();

    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scroll_callback);

    GLuint shader = CreateShaderProgram(vertexSrc, fragmentSrc);
    glUseProgram(shader);

    GLint modelLoc      = glGetUniformLocation(shader, "model");
    GLint projectionLoc = glGetUniformLocation(shader, "projection");

    // Matriz de proyección
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 750000.0f
    );
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // --- Constantes de escala ---
    const float ORBIT_SCALE = 1.0f / 3e12f;  // Disminuye distancias orbitales
    const float SIZE_SCALE  = 1.0f / 1e10f;        // Disminuye radios de planetas
    // --- Celestial Bodies ---
    std::vector<CelestialBody> bodies;

// Sol (posición en el origen)
bodies.emplace_back(glm::vec3(0.0f), 0.0f * ORBIT_SCALE, 1.989e30f, 1408.0f, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)); // Sol

// Planetas con posiciones orbitales escaladas
bodies.emplace_back(glm::vec3(0.0f), 57909227.0f * ORBIT_SCALE, 3.30e23f, 5427.0f, glm::vec4(0.8f, 0.5f, 0.2f, 1.0f));  // Mercurio
bodies.emplace_back(glm::vec3(0.0f), 108209475.0f * ORBIT_SCALE, 4.87e24f, 5243.0f, glm::vec4(1.0f, 0.9f, 0.5f, 1.0f)); // Venus
bodies.emplace_back(glm::vec3(0.0f), 149597870.0f * ORBIT_SCALE, 5.972e24f, 5514.0f, glm::vec4(0.3f, 0.5f, 1.0f, 1.0f)); // Tierra
bodies.emplace_back(glm::vec3(0.0f), 227943824.0f * ORBIT_SCALE, 6.42e23f, 3933.0f, glm::vec4(1.0f, 0.3f, 0.3f, 1.0f)); // Marte
bodies.emplace_back(glm::vec3(0.0f), 778340821.0f * ORBIT_SCALE, 1.90e27f, 1326.0f, glm::vec4(1.0f, 0.9f, 0.6f, 1.0f)); // Júpiter
bodies.emplace_back(glm::vec3(0.0f), 1426666422.0f * ORBIT_SCALE, 5.68e26f, 687.0f, glm::vec4(1.0f, 0.85f, 0.5f, 1.0f)); // Saturno
bodies.emplace_back(glm::vec3(0.0f), 2870658186.0f * ORBIT_SCALE, 8.68e25f, 1271.0f, glm::vec4(0.4f, 0.8f, 1.0f, 1.0f)); // Urano
bodies.emplace_back(glm::vec3(0.0f), 4498396441.0f * ORBIT_SCALE, 1.02e26f, 1638.0f, glm::vec4(0.3f, 0.5f, 1.0f, 1.0f)); // Neptuno

// Asignar velocidades de órbita (radianes por segundo aprox., no realistas)
bodies[1].SetOrbitSpeed(glm::radians(50.0f)); // Mercurio
bodies[2].SetOrbitSpeed(glm::radians(30.0f)); // Venus
bodies[3].SetOrbitSpeed(glm::radians(20.0f)); // Tierra
bodies[4].SetOrbitSpeed(glm::radians(15.0f)); // Marte
bodies[5].SetOrbitSpeed(glm::radians(5.0f));  // Júpiter
bodies[6].SetOrbitSpeed(glm::radians(3.0f));  // Saturno
bodies[7].SetOrbitSpeed(glm::radians(2.0f));  // Urano
bodies[8].SetOrbitSpeed(glm::radians(1.0f));  // Neptuno


    // --- OBJETOS GLOBALES ---
    objs.clear();
    // Ejemplo: Luna y Tierra como objetos simulados
    objs.emplace_back(glm::vec3(3844.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 228.0f), static_cast<float>(7.34767309e22), 3344.0f);
    objs.emplace_back(glm::vec3(0.0f), glm::vec3(0.0f), static_cast<float>(5.97219e24), 5515.0f);

    // --- GRID ---
    GLuint gridVAO, gridVBO;
    std::vector<float> gridVertices = CreateGridVertices(100000.0f, 50, objs);
    CreateVBOVAO(gridVAO, gridVBO, gridVertices.data(), gridVertices.size());

    while (!glfwWindowShouldClose(window) && running) {
        // Tiempo
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Limpieza y eventos
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();

        // Actualizar nave y cámara
        space.Update(deltaTime);
        cameraPos = space.position + glm::vec3(0.0f, 50.0f, 150.0f);
        cameraFront = glm::normalize(space.direction);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        GLint viewLoc = glGetUniformLocation(shader, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // Dibujar objetos
        for (auto& planet : bodies) {
            planet.UpdateAnimation(deltaTime);
            glm::mat4 model = planet.GetModelMatrix();
            GLint viewLoc = glGetUniformLocation(shader, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            planet.Draw(shader);
        }

        // Dibujar nave
        space.Draw(shader);

        glfwSwapBuffers(window);
    }


    // Clean-up
    for (auto& b : bodies) {
        glDeleteVertexArrays(1, &b.VAO);
        glDeleteBuffers(1, &b.VBO);
    }
    for (auto& o : objs) {
        glDeleteVertexArrays(1, &o.VAO);
        glDeleteBuffers(1, &o.VBO);
    }
    glDeleteVertexArrays(1, &gridVAO);
    glDeleteBuffers(1, &gridVBO);
    glfwTerminate();
    return 0;
}
