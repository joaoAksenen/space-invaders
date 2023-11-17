#pragma once

#include "../util/dynamic-array.h"
#include "../util/types.h"
#include "scene.h"

typedef struct SceneManager {
    DynamicArray* scenes;
    Scene* currentScene;
} SceneManager;


void _sceneManagerInit();
void _sceneManagerTerminate();
SceneManager* _sceneManagerGet();