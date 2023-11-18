#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer/renderer.h"
#include "scene/scene-manager.h"
#include "time/time.h"
#include "util/types.h"


typedef struct Engine {
    GLFWwindow* window;
    SceneManager* sceneManager;
    bool mainloopRunning;
} Engine;

bool engineInit(const char* windowTitle);
void engineMainloop();
void engineTerminate();