#include "engine/engine.h"

f32 x = -50;
void onUpdate(f32 delta) {
    engineRendererDrawQuad(x, 240, 100, 100);

    x += delta * 100;

    if (x > 690) {
        x = -50;
    } 
}

int main() {
    bool init = engineInit("Space Invaders");
    if (!init) return -1;

    engineSceneCreate("scene 1", onUpdate, NULL, NULL);

    engineMainloop();
    engineTerminate();

    return 0;
}