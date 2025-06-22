#include "globals.h"
#include "spaceship.h"
#include "functions.h"
#include "object.h"  // Ahora sí necesitas la definición completa de Object aquí.
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

const double G = 6.6743e-11; // m^3 kg^-1 s^-2
const float c = 299792458.0;

struct InputActions {
    bool moveForward  = false;
    bool moveBack     = false;
    bool moveLeft     = false;
    bool moveRight    = false;
    bool moveUp       = false;
    bool moveDown     = false;
    bool quit         = false;
    // Puedes agregar más banderas según necesites
};
InputActions actions;

extern Spaceship space;

GLFWwindow* StartGLU() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW, panic" << std::endl;
        return nullptr;
    }
    GLFWwindow* window = glfwCreateWindow(800, 600, "3D_TEST", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW." << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 800, 600);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Standard blending for transparency

    return window;
}

GLuint CreateShaderProgram(const char* vSrc, const char* fSrc) {
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vSrc, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
    }

    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fSrc, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
    }

    // Shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void CreateVBOVAO(GLuint& VAO, GLuint& VBO, const float* vertices, size_t count) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void UpdateCam(GLuint shader, const glm::vec3& camPos,
               const glm::vec3& camFront, const glm::vec3& camUp)
{ glUseProgram(shader);
    glm::mat4 view = glm::lookAt(camPos, camPos + camFront, camUp);
    GLint viewLoc = glGetUniformLocation(shader, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));}

void DrawGrid(GLuint shader, GLuint VAO, size_t count) {
    glUseProgram(shader);
    glm::mat4 model = glm::mat4(1.0f); // Identity matrix for the grid
    GLint modelLoc = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glPointSize(5.0f);
    glDrawArrays(GL_LINES, 0, count / 3);
    glBindVertexArray(0);
}

std::vector<float> CreateGridVertices(float size, int divisions, const std::vector<Object>& objs) {
    std::vector<float> vertices;
    float step = size / divisions;
    float halfSize = size / 2.0f;

    // x axis
    for (int yStep = 3; yStep <= 3; ++yStep) {
        float y = -halfSize*0.3f + yStep * step;
        for (int zStep = 0; zStep <= divisions; ++zStep) {
            float z = -halfSize + zStep * step;
            for (int xStep = 0; xStep < divisions; ++xStep) {
                float xStart = -halfSize + xStep * step;
                float xEnd = xStart + step;
                vertices.push_back(xStart); vertices.push_back(y); vertices.push_back(z);
                vertices.push_back(xEnd);   vertices.push_back(y); vertices.push_back(z);
            }
        }
    }

    // // yzxis
    // for (int xStep = 0; xStep <= divisions; ++xStep) {
    //     float x = -halfSize + xStep * step;
    //     for (int zStep = 0; zStep <= divisions; ++zStep) {
    //         float z = -halfSize + zStep * step;s
    //         for (int yStep = 0; yStep < divisions; ++yStep) {
    //             float yStart = -halfSize + yStep * step;
    //             float yEnd = yStart + step;
    //             vertices.push_back(x); vertices.push_back(yStart); vertices.push_back(z);
    //             vertices.push_back(x); vertices.push_back(yEnd);   vertices.push_back(z);
    //         }
    //     }
    // }

    // zaxis
    for (int xStep = 0; xStep <= divisions; ++xStep) {
        float x = -halfSize + xStep * step;
        for (int yStep = 3; yStep <= 3; ++yStep) {
            float y = -halfSize*0.3f + yStep * step;
            for (int zStep = 0; zStep < divisions; ++zStep) {
                float zStart = -halfSize + zStep * step;
                float zEnd = zStart + step;
                vertices.push_back(x); vertices.push_back(y); vertices.push_back(zStart);
                vertices.push_back(x); vertices.push_back(y); vertices.push_back(zEnd);
            }
        }
    }


    // displacement
    // for (int i = 0; i < vertices.size(); i += 3) {
    //     glm::vec3 vertexPos(vertices[i], vertices[i+1], vertices[i+2]);
    //     glm::vec3 totalDisplacement(0.0f);

    //     for (const auto& obj : objs) {
    //         glm::vec3 toObject = obj.GetPos() - vertexPos;
    //         float distance = glm::length(toObject);

    //         float distance_m = distance * 1000.0f;

    //         float strength = (G * obj.mass) / (distance_m * distance_m);
    //         glm::vec3 displacement = glm::normalize(toObject) * strength;

    //         totalDisplacement += -displacement * (2/distance);
    //     }

    //     vertexPos += totalDisplacement;

    //     // Update vertex data
    //     vertices[i]   = vertexPos[0];
    //     vertices[i+1] = vertexPos[1];
    //     vertices[i+2] = vertexPos[2];
    // }
    float minz = 0.0f;
    for (int i = 0; i < vertices.size(); i += 3) {
        glm::vec3 vertexPos(vertices[i], vertices[i+1], vertices[i+2]);
        glm::vec3 totalDisplacement(0.0f);


        for (const auto& obj : objs) {
            glm::vec3 toObject = obj.GetPos() - vertexPos;
            float distance = glm::length(toObject);

            float distance_m = distance * 1000.0f;
            float rs = (2*G*obj.mass)/(c*c);

            float z = 2 * sqrt(rs*(distance_m - rs)) * 100.0f;
            totalDisplacement += z;


        }

        vertexPos += totalDisplacement;

         vertices[i+1] = vertexPos[1] / 15.0f - 3000.0f;
    }


    return vertices;
}

glm::vec3 sphericalToCartesian(float r, float theta, float phi){
    float x = r * sin(theta) * cos(phi);
    float y = r * cos(theta);
    float z = r * sin(theta) * sin(phi);
    return glm::vec3(x, y, z);
};

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    space.ProcessKeyInput(key, action);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        running = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
    if (button == GLFW_MOUSE_BUTTON_LEFT){
        if (action == GLFW_PRESS){
            objs.emplace_back(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0f, 0.0f, 0.0f), initMass);
            objs[objs.size()-1].Initalizing = true;
        };
        if (action == GLFW_RELEASE){
            objs[objs.size()-1].Initalizing = false;
            objs[objs.size()-1].Launched = true;
        };
    };
    // if (!objs.empty() && button == GLFW_MOUSE_BUTTON_RIGHT && objs[objs.size()-1].Initalizing) {
    //     if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    //         objs[objs.size()-1].mass *= 1.2;}
    //         std::cout<<"MASS: "<<objs[objs.size()-1].mass<<std::endl;
    // }
};
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    float cameraSpeed = 50000.0f * deltaTime;
    if(yoffset>0){
        cameraPos += cameraSpeed * cameraFront;
    } else if(yoffset<0){
        cameraPos -= cameraSpeed * cameraFront;
    }
}