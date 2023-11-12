#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "util/types.h"

typedef struct __Engine Engine;

bool engineInit(const char* windowTitle);
void engineMainloop();
void engineTerminate();

GLFWwindow* engineGetWindow();
