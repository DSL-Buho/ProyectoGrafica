// globals.h
#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "object.h"

extern std::vector<Object> objs;
extern glm::vec3 cameraPos, cameraFront, cameraUp;
extern float lastX, lastY, yaw, pitch, deltaTime, lastFrame;
extern bool running, pause;
extern float initMass;
