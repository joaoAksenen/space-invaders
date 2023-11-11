#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine.h"


typedef struct __Engine {
    GLFWwindow* window;
} __Engine;


Engine* engine;
bool engineInit(const char* windowTitle) {
    if (!glfwInit()) return false;

    engine = malloc(sizeof(Engine));
    if (!engine) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    engine->window = glfwCreateWindow(640, 480, windowTitle, NULL, NULL);
    if (!engine->window) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(engine->window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return false;

    glViewport(0, 0, 640, 480);
    glClearColor(0.169f, 0.114f, 0.169f, 1);

    return true;
}

void engineMainloop() {
    while(!glfwWindowShouldClose(engine->window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(engine->window);
        glfwPollEvents();    
    }
}

void engineTerminate() {
    free(engine);
    glfwTerminate();
}


GLFWwindow* engineGetWindow() {
    return engine->window;
}