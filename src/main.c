#include "engine/engine.h"
#include <stdio.h>

f32 lastTime;
void onInit() {
    printf("onInit\n");
    printf("initial time: %f\n", engineTimeGetTime());
}

void onUpdate(f32 delta) {
    lastTime += delta;

    if (lastTime >= 1) {
        printf("current time after one sec: %f\n", engineTimeGetTime());
        lastTime = 0;
    }
}

void onEnd() {
    printf("onEnd\n");
}

int main() {
    bool init = engineInit("Space Invaders");
    if (!init) return -1;

    engineSceneCreate("scene 1", onUpdate, onInit, onEnd);

    engineMainloop();
    engineTerminate();

    return 0;
}