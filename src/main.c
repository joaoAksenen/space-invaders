#include "engine/engine.h"
#include <stdio.h>

void onInit() {
    printf("onInit\n");
}

int i = 0;
void onUpdate(f32 delta) {
    printf("onUpdate (1): %f\n", delta);
    if (++i >= 3) {
        puts("Changing to scene 2");
        engineSceneChange("scene 2");
    }
}

int j = 0;
void onUpdate2(f32 delta) {
    printf("onUpdate (2): %f\n", delta);
    if (++j >= 3) {
        engineTerminate();
    }
}

void onEnd() {
    printf("onEnd\n");
}

int main() {
    bool init = engineInit("Space Invaders");
    if (!init) return -1;

    engineSceneCreate("scene 1", onUpdate, onInit, onEnd);
    engineSceneCreate("scene 2", onUpdate2, onInit, onEnd);

    engineSceneChange("scene 2");
    engineSceneChange("scene 1");

    engineMainloop();
    engineTerminate();

    return 0;
}