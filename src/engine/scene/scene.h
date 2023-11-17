#pragma once

#include "../util/types.h"

typedef struct Scene {
    void (*onStart)();
    void (*onUpdate)(f32);
    void (*onEnd)();

    const char *name;
} Scene;


size_t engineSceneCreate(const char *name, void (*onUpdate)(f32), void (*onStart)(), void (*onEnd)());
size_t engineSceneDelete(const char* name);
void engineSceneChange(const char* name);
bool engineSceneExists(const char* name);
