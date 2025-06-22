#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "object.h"
// o, si solo necesitas referencia:
class Object;


// Inicialización de GLFW, GLEW y estado GL
GLFWwindow* StartGLU();

// Compilación y linking de shaders
GLuint CreateShaderProgram(const char* vSrc, const char* fSrc);

// Creación de un VAO/VBO para datos de vértices (solo posición xyz)
void CreateVBOVAO(GLuint& VAO, GLuint& VBO, const float* vertices, size_t count);

// Actualización de la vista de cámara (uniform "view")
void UpdateCam(GLuint shader, const glm::vec3& camPos,
               const glm::vec3& camFront, const glm::vec3& camUp);

// Dibuja una cuadrícula usando líneas
void DrawGrid(GLuint shader, GLuint VAO, size_t count);

// Genera vértices para una cuadrícula con desplazamiento
std::vector<float> CreateGridVertices(float size, int divisions, const std::vector<Object>& objs);

// Conversión esférica → cartesiana
glm::vec3 sphericalToCartesian(float r, float theta, float phi);

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);