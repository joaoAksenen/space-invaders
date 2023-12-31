#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer/renderer-internal.h"
#include "scene/scene-manager.h"
#include "time/time-internal.h"
#include "util/error.h"
#include "engine.h"

Engine* engine;

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480


static void windowSizeCallback(GLFWwindow* window, int width, int height);

bool engineInit(const char* windowTitle) {
    if (!glfwInit()) ERROR_RETURN(false, "Unable to initialize glfw\n");

    engine = malloc(sizeof(Engine));
    if (!engine) ERROR_RETURN(false, "Unable to allocate memory for engine\n");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    engine->window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, windowTitle, NULL, NULL);
    if (!engine->window) {
        glfwTerminate();
        ERROR_RETURN(false, "Unable to create window\n");
    }

    glfwMakeContextCurrent(engine->window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        ERROR_RETURN(false, "Unable to initialize glad (opengl)\n");
    }

    _rendererInit(WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetWindowSizeCallback(engine->window, windowSizeCallback);

    _timeInit(60);

    glClearColor(0.169f, 0.114f, 0.169f, 1);

    _sceneManagerInit();
    engine->sceneManager = _sceneManagerGet();
    engine->mainloopRunning = false;

    return true;
}

void engineMainloop() {
    engine->mainloopRunning = true;

    SceneManager* sm = engine->sceneManager;

    if (sm->scenes->length == 0)
        ERROR_EXIT("No scenes to run\n");
    if (!sm->currentScene->onUpdate)
        ERROR_EXIT("No onUpdate function for current scene\n");
    
    if (sm->currentScene->onStart)
        sm->currentScene->onStart();

    while(!glfwWindowShouldClose(engine->window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        sm->currentScene->onUpdate(_timeUpdate());

        glfwSwapBuffers(engine->window);
        glfwPollEvents();    
    }

    if (sm->currentScene->onEnd)
        sm->currentScene->onEnd();

    engine->mainloopRunning = false;
}

void engineTerminate() {
    if (engine->mainloopRunning && engine->sceneManager->currentScene->onEnd)
        engine->sceneManager->currentScene->onEnd();

    engine->mainloopRunning = false;
    _sceneManagerTerminate();
    free(engine);

    glfwTerminate();
}

static void windowSizeCallback(GLFWwindow* window, int width, int height) {
    _rendererUpdateViewport(width, height);
}