// globals.cpp
#include "globals.h"
std::vector<Object> objs;
glm::vec3 cameraPos = glm::vec3(0.0f, 1000.0f, 5000.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float lastX = 400.0f, lastY = 300.0f;
float yaw = -90.0f, pitch = 0.0f, deltaTime = 0.0f, lastFrame = 0.0f, initMass = 1e20f;
bool running = true, pause = false;

