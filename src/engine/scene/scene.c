#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../util/error.h"
#include "../engine.h"

#include "scene-manager.h"
#include "scene.h"

SceneManager sceneManager = {0};
extern Engine *engine;

void _sceneManagerInit() {
    sceneManager.scenes = dynamicArrayCreate(sizeof(Scene), 8);
}

void _sceneManagerTerminate() {
    for (size_t i = 0; i < sceneManager.scenes->length; i++) {
        free(dynamicArrayGet(sceneManager.scenes, i));
    }

    dynamicArrayDestroy(sceneManager.scenes);
}

SceneManager* _sceneManagerGet() {
    return &sceneManager;
}

size_t engineSceneCreate(const char *name, void (*up)(f32), void (*st)(), void (*en)()) {
    if (engineSceneExists(name)) 
        ERROR_EXIT(0, "A scene with the name \"%s\" already exists.\n", name);

    Scene *newScene = (Scene*) malloc(sizeof(Scene));
    if (!newScene) ERROR_RETURN(0, "Unable to create new scene");

    newScene->name = name;
    newScene->onStart = st;
    newScene->onUpdate = up;
    newScene->onEnd = en;

    if (sceneManager.scenes->length == 0) sceneManager.currentScene = newScene;

    return dynamicArrayAppend(sceneManager.scenes, newScene) + 1;
}

size_t engineSceneDelete(const char *name) {
    for (size_t i = 0; i < sceneManager.scenes->length; i++) {
        Scene *curr = (Scene*)dynamicArrayGet(sceneManager.scenes, i);
        if (strcmp(name, curr->name) == 0) {
            if (strcmp(name, sceneManager.currentScene->name) == 0) 
                return 1;

            dynamicArrayRemove(sceneManager.scenes, i);
            break;
        }
    }

    return 0;
}

void engineSceneChange(const char *name) {
    Scene *nextScene = NULL;

    for (size_t i = 0; i < sceneManager.scenes->length; i++) {
        Scene *curr = (Scene*)dynamicArrayGet(sceneManager.scenes, i);
        if (strcmp(curr->name, name) == 0) {
            nextScene = curr;
            break;
        }
    }

    if (nextScene == NULL) ERROR_EXIT("No scene with the name \"%s\" was found.\n", name);

    if (engine->mainloopRunning && &sceneManager.currentScene->onEnd != NULL)
        (*sceneManager.currentScene->onEnd)();

    sceneManager.currentScene = nextScene;

    sceneManager.currentScene->onStart;
    if (engine->mainloopRunning && &sceneManager.currentScene->onStart != NULL)
        (*sceneManager.currentScene->onStart)();
}

bool engineSceneExists(const char* name) {
    for (size_t i = 0; i < sceneManager.scenes->length; i++) {
        Scene *curr = (Scene*)dynamicArrayGet(sceneManager.scenes, i);
        if (strcmp(curr->name, name) == 0) {
            return true;
        }
    }

    return false;
}